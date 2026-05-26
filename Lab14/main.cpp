#include <iostream> 
#include <thread>

using namespace std;

void verHilosPID(){

}

int main()
{
    while (true){
        int opcion;
        cout << "\n==== INSPECTOR DE PROCESOS ====" << endl;
        cout << "\n1. Deseo inspeccionar un proceso a partir de su nombre.\n2.Deseo inspeccionar un proceso a partir de su PID\n3.Salir" << endl;
        cout << "Ingrese una opción: " << endl;

        switch (opcion){
            case 1:
                "TODO"
                break;
            case 2:
                "TODO"
                break;
            case 3:
                cout << "\nPrograma finalizado, adios." << endl;
                return;
        }
    }
}

/*
    Este programa funciona únicamente en linux y depende de htop, es necesario instalarlo (desde ubuntu: sudo apt install htop)
    
*/