#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct TrabajoImpresion {
    int id;
    string nombreArchivo;
    int paginas;
};

void agregarTrabajo(vector<TrabajoImpresion>& cola, int& siguienteId) {
    string archivo;
    int paginas;

    cout << "Nombre del archivo: ";
    cin >> archivo;

    do{
        cout << "Cantidad de paginas: ";
        cin >> paginas;

        if (paginas<=0) cout << "Jakdj me intentaba romper el programa, segui probando...\n";
    } while (paginas<=0);

    cola.push_back({siguienteId++, archivo, paginas});
    cout << "Trabajo agregado correctamente.\n";
}

void listarTrabajos(const vector<TrabajoImpresion>& cola) {
    if (cola.empty()) {
        cout << "No hay trabajos pendientes.\n";
        return;
    }

    cout << "\n=== TRABAJOS PENDIENTES ===\n";

    for (const TrabajoImpresion& trabajo : cola) {
        cout << "ID: " << trabajo.id << " | Archivo: " << trabajo.nombreArchivo << " | Paginas: " << trabajo.paginas << endl;
    }
}

void procesarSiguiente(vector<TrabajoImpresion>& cola) {
    if (cola.empty()) {
        cout << "La cola de impresion esta vacia.\n";
        return;
    }

    TrabajoImpresion trabajo = cola.front();
    cola.erase(cola.begin());

    cout << "Procesando: " << trabajo.nombreArchivo << " (" << trabajo.paginas << " paginas)\n";
}

void cancelarTrabajo(vector<TrabajoImpresion>& cola) {
    int id;

    cout << "Ingrese ID a cancelar: ";
    cin >> id;

    for (vector<TrabajoImpresion>::iterator it = cola.begin(); it != cola.end(); ++it) {
        if (it->id == id) {
            cola.erase(it);
            cout << "Trabajo cancelado.\n";
            return;
        }
    }

    cout << "No se encontro un trabajo con ese ID.\n";
}

void mostrarEstadisticas(const vector<TrabajoImpresion>& cola) {
    int totalPaginas = 0;

    for (const TrabajoImpresion& trabajo : cola) {
        totalPaginas += trabajo.paginas;
    }

    cout << "\n=== ESTADISTICAS ===\n";
    cout << "Trabajos pendientes: " << cola.size() << endl;
    cout << "Paginas en espera: " << totalPaginas << endl;
}

int main() {
    vector<TrabajoImpresion> cola;
    int opcion;
    int siguienteId = 1;

    do {
        cout << "\n===== COLA DE IMPRESION =====\n";
        cout << "1. Agregar trabajo\n";
        cout << "2. Listar trabajos\n";
        cout << "3. Procesar siguiente trabajo\n";
        cout << "4. Cancelar trabajo\n";
        cout << "5. Mostrar estadisticas\n";
        cout << "6. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarTrabajo(cola, siguienteId);
                break;

            case 2:
                listarTrabajos(cola);
                break;

            case 3:
                procesarSiguiente(cola);
                break;

            case 4:
                cancelarTrabajo(cola);
                break;

            case 5:
                mostrarEstadisticas(cola);
                break;

            case 6:
                cout << "Saliendo del sistema...\n";
                break;

            default:
                cout << "Opcion invalida.\n";
        }

    } while (opcion != 6);

    return 0;
}