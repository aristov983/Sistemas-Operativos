#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

class Archivo {
private:
    string nombre;
    size_t tamanio;

public:
    Archivo(string nombre, size_t tamanio) : nombre(nombre), tamanio(tamanio) {}

    string getNombre() const {
        return nombre;
    }

    size_t getTamanio() const {
        return tamanio;
    }
};

class MemoriaManager {
private:
    size_t memoriaUso;
    size_t capacidad;
    vector<Archivo> archivosMemoria;

public:
    static const size_t KB = 1024;

    MemoriaManager(size_t capacidad)
        : memoriaUso(0), capacidad(capacidad) {}

    size_t getCapacidad() const {
        return capacidad;
    }

    size_t getMemoriaUso() const {
        return memoriaUso;
    }

    size_t getMemoriaLibre() const {
        return capacidad - memoriaUso;
    }

    double getPorcentajeUso() const {
        return capacidad == 0
            ? 0
            : (memoriaUso * 100.0) / capacidad;
    }

    bool estaCargado(const string& nombre) const {
        for (const Archivo& a : archivosMemoria) {
            if (a.getNombre() == nombre) {
                return true;
            }
        }
        return false;
    }

    void cargarEnMemoria(const Archivo& archivo) {

        if (estaCargado(archivo.getNombre())) {
            cout << "\nERROR: EL ARCHIVO YA ESTA CARGADO.\n";
            return;
        }

        if (archivo.getTamanio() > getMemoriaLibre()) {

            cout << "\nERROR: NO HAY MEMORIA SUFICIENTE.\n";
            cout << "Memoria disponible: " << getMemoriaLibre() << " bytes\n";

            cout << "Tamaño del archivo: " << archivo.getTamanio()<< " bytes\n";

            return;
        }

        archivosMemoria.push_back(archivo);
        memoriaUso += archivo.getTamanio();

        cout << "\nArchivo cargado correctamente.\n";
    }

    void liberarDeMemoria(const string& nombre) {

        for (auto it = archivosMemoria.begin(); it != archivosMemoria.end(); ++it) {
            if (it->getNombre() == nombre) {
                memoriaUso -= it->getTamanio();
                archivosMemoria.erase(it);

                cout << "\nArchivo liberado correctamente.\n";
                return;
            }
        }

        cout << "\nERROR: EL ARCHIVO NO ESTA CARGADO.\n";
    }

    void mostrarEstado() const {

        cout << "\n===== ESTADO DE MEMORIA =====\n";

        cout << "Memoria total: " << capacidad << " bytes\n";

        cout << "Memoria utilizada: " << memoriaUso << " bytes\n";

        cout << "Memoria libre: " << getMemoriaLibre() << " bytes\n";

        cout << "Uso de memoria: " << getPorcentajeUso() << "%\n";

        cout << "\nArchivos cargados:\n";

        if (archivosMemoria.empty()) {
            cout << "(ninguno)\n";
        } else {
            for (const Archivo& a : archivosMemoria) {
                cout << "- " << a.getNombre() << " | " << a.getTamanio() << " bytes\n";
            }
        }
    }
};

vector<Archivo> obtenerArchivosDisco() {

    vector<Archivo> archivos;
    fs::path carpeta("archivos");
    if (!fs::exists(carpeta)) {
        return archivos;
    }

    for (const auto& entrada : fs::directory_iterator(carpeta)) {
        if (entrada.is_regular_file()) {
            archivos.emplace_back(
                entrada.path().filename().string(),
                entrada.file_size()
            );
        }
    }

    return archivos;
}

void listarArchivosDisco() {
    vector<Archivo> archivos = obtenerArchivosDisco();
    cout << "\n===== ARCHIVOS DISPONIBLES =====\n";
    for (const Archivo& a : archivos) {
        double kb = a.getTamanio() / 1024.0;
        cout << "- " << a.getNombre() << " | "<< a.getTamanio() << " bytes | " << kb << " KB\n";
    }
}

bool buscarArchivo(
    const string& nombre,
    Archivo& resultado) {

    vector<Archivo> archivos = obtenerArchivosDisco();

    for (const Archivo& a : archivos) {
        if (a.getNombre() == nombre) {
            resultado = a;
            return true;
        }
    }

    return false;
}

void imprimirMenu() {

    cout << "\n===== SIMULADOR DE MEMORIA Y ARCHIVOS =====\n";
    cout << "1. Listar archivos disponibles en disco\n";
    cout << "2. Cargar archivo en memoria\n";
    cout << "3. Liberar archivo de memoria\n";
    cout << "4. Ver estado de la memoria\n";
    cout << "5. Salir\n";
}

int main() {

    const size_t MEMORIA_TOTAL = 100 * 1024; // 100 KB

    MemoriaManager memoria(MEMORIA_TOTAL);

    int opcion;

    do {
        imprimirMenu();

        cout << "\nSeleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {

        case 1:
            listarArchivosDisco();
            break;

        case 2: {
            string nombre;

            cout << "Ingrese nombre del archivo: ";
            cin >> nombre;

            Archivo archivo("", 0);

            if (buscarArchivo(nombre, archivo)) {
                memoria.cargarEnMemoria(archivo);
            } else {
                cout << "\nERROR: EL ARCHIVO NO EXISTE.\n";
            }

            break;
        }

        case 3: {
            string nombre;

            cout << "Ingrese nombre del archivo: ";
            cin >> nombre;

            memoria.liberarDeMemoria(nombre);

            break;
        }

        case 4:
            memoria.mostrarEstado();
            break;

        case 5:
            cout << "\nFinalizando programa...\n";
            break;

        default:
            cout << "\nOpcion invalida.\n";
        }
    } while (opcion != 5);
    return 0;
}