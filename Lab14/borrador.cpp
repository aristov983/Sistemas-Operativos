```cpp
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <csignal>
#include <pthread.h>
#include <unistd.h>

using namespace std;

struct Hilo {
    int tid;
    string estado;
};

// Obtener nombre del proceso
string obtenerNombreProceso(int pid) {
    string ruta = "/proc/" + to_string(pid) + "/comm";

    ifstream archivo(ruta); //En archivo leo todo lo que está en la ruta

    string nombre;

    if (archivo.is_open()) {
        getline(archivo, nombre);
        archivo.close();
    }

    return nombre;
}

// Buscar PID por nombre
int buscarPIDPorNombre(string nombreBuscado) {
    DIR* dir = opendir("/proc");

    if (dir == NULL) {
        return -1;
    }

    struct dirent* entrada;

    while ((entrada = readdir(dir)) != NULL) {

        // Verificar si es un PID
        if (entrada->d_type == DT_DIR) {

            int pid = atoi(entrada->d_name);

            if (pid > 0) {

                string nombre = obtenerNombreProceso(pid);

                if (nombre == nombreBuscado) {
                    closedir(dir);
                    return pid;
                }
            }
        }
    }

    closedir(dir);

    return -1;
}

// Obtener estado de un hilo
string obtenerEstadoHilo(int pid, int tid) {

    string ruta = "/proc/" + to_string(pid) +
                  "/task/" + to_string(tid) + "/status";

    ifstream archivo(ruta);

    string linea;

    while (getline(archivo, linea)) {

        if (linea.find("State:") == 0) {
            return linea;
        }
    }

    return "Estado desconocido";
}

// Obtener lista de hilos
vector<Hilo> obtenerHilos(int pid) {

    vector<Hilo> hilos;

    string ruta = "/proc/" + to_string(pid) + "/task";

    DIR* dir = opendir(ruta.c_str());

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

// Exportar a TXT
void exportarTXT(string nombreProceso, int pid, vector<Hilo> hilos) {

    ofstream archivo("reporte_hilos.txt");

    archivo << "Proceso: " << nombreProceso << endl;
    archivo << "PID: " << pid << endl;
    archivo << endl;

    archivo << "Lista de hilos:" << endl;

    for (auto h : hilos) {

        archivo << "TID: " << h.tid
                << " - " << h.estado << endl;
    }

    archivo.close();

    cout << "\nListado exportado a reporte_hilos.txt\n";
}

// Terminar hilo
void terminarHilo(int tid) {

    int resultado = pthread_kill((pthread_t)tid, SIGKILL);

    if (resultado == 0) {
        cout << "El hilo " << tid
             << " ha sido terminado con éxito.\n";
    }
    else {
        cout << "Error al terminar el hilo.\n";
    }
}

int main() {

    int opcion;

    cout << "1 - Buscar por nombre\n";
    cout << "2 - Ingresar PID\n";
    cout << "Opcion: ";
    cin >> opcion;

    int pid;

    if (opcion == 1) {

        string nombre;

        cout << "Ingrese nombre del proceso: ";
        cin >> nombre;

        pid = buscarPIDPorNombre(nombre);

        if (pid == -1) {
            cout << "Proceso no encontrado.\n";
            return 0;
        }
    }
    else {

        cout << "Ingrese PID: ";
        cin >> pid;
    }

    string nombreProceso = obtenerNombreProceso(pid);

    cout << "\nProceso encontrado\n";
    cout << "Nombre: " << nombreProceso << endl;
    cout << "PID: " << pid << endl;

    vector<Hilo> hilos = obtenerHilos(pid);

    cout << "\nHilos activos:\n";

    for (auto h : hilos) {

        cout << "TID: " << h.tid
             << " - " << h.estado << endl;
    }

    exportarTXT(nombreProceso, pid, hilos);

    int opcionFinalizar;

    cout << "\nDesea terminar un hilo?\n";
    cout << "1 - Si\n";
    cout << "2 - No\n";
    cin >> opcionFinalizar;

    if (opcionFinalizar == 1) {

        int tid;

        cout << "Ingrese TID: ";
        cin >> tid;

        terminarHilo(tid);
    }

    return 0;
}
```
