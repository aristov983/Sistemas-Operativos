#include <iostream> 
#include <dirent.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <csignal>
#include <pthread.h>
#include <unistd.h>
#include <filesystem> 
#include <signal.h>

using namespace std;
namespace fs = std::filesystem;

//obtener nombre a partir de PID
string obtenerNombreProceso(int pid){
    string ruta = "/proc/" + to_string(pid) + "/comm";

    ifstream archivo(ruta); //En archivo leo todo lo que está en la ruta

    string nombre;
    if (archivo.is_open()){
        getline(archivo, nombre);
        archivo.close();
    }

    return nombre;
}

//pbtener PID a partir de nombre
int obtenerPidProceso(string nombre){
    
    DIR* d = opendir("/proc/"); //El * hace que DIR sea un puntero al espacio de memoria.
    struct dirent* entrada; //dirent es una estructura de <dirent.h> significa directory entry.
    entrada = readdir(d); //readdir(dir) devuelve el siguiente elemento del directorio cada vez que se ejecuta.
    while (entrada!=NULL){

        if (entrada->d_type == DT_DIR){ //acá usa -> porque entrada es un puntero, y checkea que la entrada sea de tipo directorio (Todo esto es de <dirent.h>)
            int pid_actual = atoi(entrada->d_name); //proc guarda carpetas y los nombres son numeros (el pid de los procesos), tonces paso ese numero a int.
            
            if (pid_actual>0){ //chequea que sea >0 porque hay otras carpetas que no son procesos y si atoi falla devuelve 0.
                if (nombre == obtenerNombreProceso(pid_actual)){
                    closedir(d);
                    return pid_actual;
                }
            }
        }

        entrada = readdir(d);
    }

    closedir(d);
    return -1;
    
}

//verificar si PID existe
bool verificarPID(int pid){
    DIR* d = opendir("/proc/");

    struct dirent* entrada;
    entrada = readdir(d);

    while (entrada!=NULL){

        if (entrada->d_type == DT_DIR){

            int subPID = atoi(entrada->d_name);
            if (subPID>0){

                if (pid == subPID){
                    closedir(d);
                    return true;
                }
            }
        }
        entrada = readdir(d);
    }

    closedir(d);
    return false;
}

//obtener estado de un hilo
string obtenerEstadoHilo(int pid, int tid) {
    string ruta = "/proc/" + to_string(pid) + "/task/" + to_string(tid) + "/status";
    ifstream archivo(ruta);
    string linea;
    while (getline(archivo, linea)) {
        if (linea.find("State:") == 0) {
            return linea;
        }
    }

    return "Estado desconocido";
}

struct Hilo {
    int tid;
    string estado;
};

//obtener lista de hilos
vector<Hilo> obtenerHilos(int pid) {

    vector<Hilo> hilos;
    string ruta = "/proc/" + to_string(pid) + "/task";
    DIR* d = opendir(ruta.c_str()); //c_str() convierte el string a char* porque opendir funciona con eso.

    if (d == NULL) {
        return hilos;
    }

    struct dirent* entrada;

    while ((entrada = readdir(d)) != NULL) {

        if (entrada->d_type == DT_DIR) {
            int tid = atoi(entrada->d_name);
            if (tid > 0) {
                Hilo h;
                h.tid = tid;
                h.estado = obtenerEstadoHilo(pid, tid);
                hilos.push_back(h);
            }
        }
    }

    closedir(d);
    return hilos;
}

//listarhilos
void listarHilos(int pid) {

    vector<Hilo> hilos = obtenerHilos(pid);

    if (hilos.empty()){
        cout << "No se encontraron hilos." << endl;
        return;
    }

    for (const Hilo& h : hilos){
        cout << "TID: " << h.tid << " | Estado: " << h.estado << endl;
    }

}

//terminar hilo
void terminarHilo(int tid) {

    int resultado = kill(tid, SIGKILL);

    if (resultado == 0) {
        cout << "El hilo " << tid << " ha sido terminado con éxito.\n";
    } else {
        cout << "Error al terminar el hilo.\n";
    }
}

//verificar si existe hilo
bool existeHilo(int pid, int tid) {

    string ruta =
        "/proc/" + to_string(pid) +
        "/task/" + to_string(tid);

    return fs::exists(ruta);
}

void imprimirMenu(){
    cout << "\n==== INSPECTOR DE PROCESOS ====" << endl;
    cout << "\n1. Deseo inspeccionar un proceso a partir de su PID.\n2.Deseo inspeccionar un proceso a partir de su nombre\n3.Salir" << endl;
    cout << "Ingrese una opción: ";
}

void imprimirSubMenuProceso(){
    cout << "\n---- Inspeccion de proceso e hilos ----" << endl;
    cout << "\n1.Deseo finalizar un hilo ingresando su TID\n2.Deseo listar los hilos del proceso y guardarlos en un txt.\n3.Volver." <<endl;
    cout << "Ingrese una opción: ";
}

void exportarTXT(vector<Hilo> hilos){

    ofstream archivo("hilos.txt");

    if (!archivo.is_open()){
        cout << "No se pudo crear el archivo." << endl;
        return;
    }

    for (const Hilo& h : hilos){

        archivo << "TID: " << h.tid << " | Estado: " << h.estado << endl;
    }

    archivo.close();

    cout << "Listado exportado correctamente a hilos.txt" << endl;
}



int main()
{
    int opcion;
    int subopcion;

    while (true){
        imprimirMenu();
        cin >> opcion;

        switch (opcion){

            case 1:{
                int pid;
                cout << "Ingrese PID: ";
                cin >> pid;
                if(verificarPID(pid)){
                    bool volver = false;
                    while (!volver){
                        imprimirSubMenuProceso();
                        cin >> subopcion;
                        switch (subopcion){
                            case 1:{
                                vector<Hilo> hilos = obtenerHilos(pid);
                                int tid;
                                cout << "Ingrese TID: ";
                                cin >> tid;
                                if (existeHilo(pid, tid)){
                                    terminarHilo(tid);
                                    break;
                                } else {
                                    cout << "No existe el hilo.";
                                    break;
                                }
                            }
                            case 2:{
                                vector<Hilo> hilos = obtenerHilos(pid);
                                exportarTXT(hilos);
                                listarHilos(pid);
                                break;
                            }
                            case 3:{
                                volver=true;
                                break;
                            }
                        }
                    }
                } else {
                    cout << "No existe proceso con ese PID" << endl;
                    break;
                }
                break;
            }

            case 2:{
                string nombre;
                cout << "Ingrese nombre del proceso: ";
                cin >> nombre;

                int pid = obtenerPidProceso(nombre);
                if (pid>0){
                    bool volver = false;
                    while (!volver){
                        imprimirSubMenuProceso();
                        cin >> subopcion;
                        switch (subopcion){
                            case 1:{
                                vector<Hilo> hilos = obtenerHilos(pid);
                                int tid;
                                cout << "Ingrese TID: ";
                                cin >> tid;
                                if (existeHilo(pid, tid)){
                                    terminarHilo(tid);
                                    break;
                                } else {
                                    cout << "No existe el hilo.";
                                    break;
                                }
                            }
                            case 2:{
                                vector<Hilo> hilos = obtenerHilos(pid);
                                listarHilos(pid);
                                exportarTXT(hilos);
                                break;
                            }
                            case 3:{
                                volver = true;
                                break;
                            }

                            default:
                                cout<<"Opcion invalida.";
                        }
                    }
                } else {
                    cout << "El proceso no existe.";
                }
                break;
            }

            case 3:{
                return 0;
            }

            default:{
                cout<<"OPCION INVALIDA NO SABES LEER DIOS HACE 3 HORAS ESTOY CON ESTO." << endl;
            }
        }
    }
    return 0;
}