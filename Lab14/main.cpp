#include <iostream> 
#include <dirent.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstrings>
#include <csingla>
#include <pthread.h>
#include <unistd.h>
#include <filesystem.h> 

using namespace std;

struct Hilo {
    int tid; //Thread id
    string estado;
}

//obtener nombre a partir de PID
string obtenerNombreProceso(int pid){
    string ruta = "/proc/" + to_string(pid) + "/comn";

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
    entrada = readdir(dir); //readdir(dir) devuelve el siguiente elemento del directorio cada vez que se ejecuta.
    while (entrada!=NULL){

        if (entrada->d_type == DT_DIR){ //acá usa -> porque entrada es un puntero, y checkea que la entrada sea de tipo directorio (Todo esto es de <dirent.h>)
            int pid_actual = atoi(entrada->d_name); //proc guarda carpetas y los nombres son numeros (el pid de los procesos), tonces paso ese numero a int.
            
            if (pid_actual>0){ //chequea que sea >0 porque hay otras carpetas que no son procesos y si atoi falla devuelve 0.
                if (nombre == obtenerNombreProceso(pid_actual)){
                    closedir(dir);
                    return pid_actual;
                }
            }
        }

        entrada = readdir(dir);
    }

    return -1;
    
}

//verificar si PID existe
bool verificarPID(int pid){
    DIR* d = opendir("/proc/");

    struct dirent* entrada;
    entrada = readdir(dir);

    while (entrada!=NULL){

        if (entrada->d_type == DT_DIR){

            int subPID = atoi(entrada->d_name);
            if (sub_PID>0){

                if (pid == sub_PID){
                    closedir(dir);
                    return true;
                }
            }
        }
        entrada = readdir(dir);
    }

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

//listarhilos
void listarHilos(int pid) {

    string ruta = "/proc/" + to_string(pid) + "/task/";

    if (!fs::exists(ruta)) {
        cout << "El proceso no existe" << endl;
        return;
    }

    cout << "\nHilos del proceso " << pid << ":" << endl;

    for (const auto& entrada : fs::directory_iterator(ruta)) {

        string tid = entrada.path().filename().string();

        cout << "TID: " << tid << endl;
    }
}

//obtener lista de hilos
vector<Hilo> obtenerHilos(int pid) {

    vector<Hilo> hilos;
    string ruta = "/proc/" + to_string(pid) + "/task";
    DIR* dir = opendir(ruta.c_str()); //c_str() convierte el string a char* porque opendir funciona con eso.

    if (dir == NULL) {
        return hilos;
    }

    struct dirent* entrada;

    while ((entrada = readdir(dir)) != NULL) {

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

    closedir(dir);
    return hilos;
}

//terminar hilo
void terminarHilo(int tid) {

    int resultado = pthread_kill((pthread_t)tid, SIGKILL);

    if (resultado == 0) {
        cout << "El hilo " << tid << " ha sido terminado con éxito.\n";
    }
    else {
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
    cout << "\n1.Deseo finalizar un hilo ingresando su TID\n2.Deseo listar los hilos del proceso\nVolver." <<endl;
    cout << "Ingrese una opción: ";
}



int main()
{
    int opcion;
    int subopcion;

    while (true){
        imprimirMenu();
        cin >> opcion;

        switch (opcion){

            case 1:
                int pid;
                cout << "Ingrese PID: "
                cin >> pid;
                if(verificarPID(pid)){
                    while (true){
                        imprimirSubMenuProceso();
                        cin >> subopcion;
                        switch (subopcion){
                            case 1:
                                vector<HILO> hilos = obtenerHilos(pid);
                                int tid;
                                cout << "Ingrese TID: "
                                cin >> tid;
                                if (existeHilo(pid, tid)){
                                    terminarHilo(tid);
                                    break;
                                } else {
                                    cout << "No existe el hilo."
                                    break;
                                }
                            case 2
                                listarHilos(pid);
                                break;
                            case 3:
                                return;
                        }
                    }
                } else {
                    cout << "No existe proceso con ese PID" << endl;
                    break;
                }

            case 2:
                string nombre;
                cout << "Ingrese nombre del proceso: "
                cin >> nombre;

                int pid = buscarPIDPorNombre(nombre);
                if (pid>0){
                    //LOGICA HILOS
                } else {
                    cout << "El proceso no existe."
                }
        
            case 3:
                return;
        }
    }
}

/*
    Este programa funciona únicamente en linux y depende de htop, es necesario instalarlo (desde ubuntu: sudo apt install htop)
    
*/