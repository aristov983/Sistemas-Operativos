#include <string>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <list>

using namespace std;

class actividad{
    private:
        string fecha;
        string hora;
        string descripcion;
        string codigoVerificacion;

    public:

        void setFecha(const string& nuevaFecha){
            if (actividad::validarFormatoFecha(nuevaFecha)){
                this->fecha = nuevaFecha;
            }
            else{
                throw invalid_argument("La fecha no cumple con el formato esperado.");
            }
        }

        string getFecha(){
            return this->fecha;
        }

        void setHora(const string& nuevaHora){
            if (actividad::validarFormatoHora(nuevaHora)){
                this->hora = nuevaHora;
            }
            else{
                throw invalid_argument("La hora no cumple con el formato esperado.");
            }
        }

        string getHora(){
            return this->hora;
        }

        void setDescripcion(const string& nuevaDescripcion){
            this->descripcion = nuevaDescripcion;
        }

        string getDescripcion(){
            return this->descripcion;
        }

        void setCodigoVerificacion(const string& nuevoCodigoVerificacion){
            this->codigoVerificacion = nuevoCodigoVerificacion;
        }

        string getCodigoVerificacion(){
            return this->codigoVerificacion;
        }

        static bool validarFormatoFecha(const string& fecha){
            regex patron("\\d{2}/\\d{2}/\\d{4}");
            return regex_match(fecha, patron);
        }

        static bool validarFormatoHora(const string& hora){
            regex patron("\\d{2}:\\d{2}:\\d{2}");

            if (regex_match(hora, patron)){

                int hh = stoi(hora.substr(0,2));
                int mm = stoi(hora.substr(3,2));
                int ss = stoi(hora.substr(6,2));

                if ((hh >= 0 && hh <= 23) && (mm >= 0 && mm <= 59) && (ss >= 0 && ss <= 59)){
                    return true;
                }
            }

            return false;
        }

        void imprimirActividad(){
            cout << "-- Actividad --" << endl;
            cout << "Fecha: " << this->fecha << endl;
            cout << "Hora: " << this->hora << endl;
            cout << "Descripcion: " << this->descripcion << endl;

            if (codigoVerificacion.length() != 0){
                cout << "Codigo verificacion: " << this->codigoVerificacion << endl;
            }

            cout << endl;
        }

        actividad(string fecha, string hora, string descripcion){
            this->fecha = fecha;
            this->hora = hora;
            this->descripcion = descripcion;
        }
};

list<actividad> buscarActividadFecha(const string& fechaObjetivo, const list<actividad>& listaOriginal){

    list<actividad> listaAuxiliar;

    for (actividad act : listaOriginal){

        if (act.getFecha() == fechaObjetivo){
            listaAuxiliar.push_back(act);
        }
    }

    return listaAuxiliar;
}

void listarActividades(list<actividad>& listaActividades){

    int numero = 1;

    for (actividad act : listaActividades){

        cout << "Actividad #" << numero << endl;
        act.imprimirActividad();

        numero++;
    }
}

int main(){

    int opcion;
    list<actividad> listaActividades;

    while (true){

        cout << "\n====== MENU PRINCIPAL ======" << endl;
        cout << "1. Registrar actividad." << endl;
        cout << "2. Mostrar actividades de un dia." << endl;
        cout << "3. Modificar actividad." << endl;
        cout << "4. Eliminar actividad." << endl;
        cout << "5. Salir." << endl;

        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion){

            case 1:{

                do{

                    cout << "\n-- Creacion de nueva actividad --" << endl;

                    string fecha;
                    string hora;
                    string descripcion;
                    string codigo;

                    do {

                        cout << "Ingrese la fecha (DD/MM/AAAA): ";
                        cin >> fecha;

                        if (!actividad::validarFormatoFecha(fecha)){
                            cout << "ERROR: Fecha invalida." << endl;
                        }

                    } while (!actividad::validarFormatoFecha(fecha));

                    do {

                        cout << "Ingrese la hora (HH:MM:SS): ";
                        cin >> hora;

                        if (!actividad::validarFormatoHora(hora)){
                            cout << "ERROR: Hora invalida." << endl;
                        }

                    } while (!actividad::validarFormatoHora(hora));

                    cin.ignore();

                    cout << "Ingrese descripcion: ";
                    getline(cin, descripcion);

                    actividad nuevaActividad(fecha, hora, descripcion);

                    do {

                        cout << "Desea agregar codigo de verificacion?" << endl;
                        cout << "1. Si" << endl;
                        cout << "2. No" << endl;

                        cin >> opcion;

                        if (opcion != 1 && opcion != 2){
                            cout << "Opcion invalida." << endl;
                        }

                    } while (opcion != 1 && opcion != 2);

                    if (opcion == 1){

                        cin.ignore();

                        do{

                            cout << "Ingrese codigo: ";
                            getline(cin, codigo);

                            if (codigo.length() == 0){
                                cout << "Codigo invalido." << endl;
                            }

                        } while (codigo.length() == 0);

                        nuevaActividad.setCodigoVerificacion(codigo);
                    }

                    listaActividades.push_back(nuevaActividad);

                    do {

                        cout << "\nDesea agregar otra actividad?" << endl;
                        cout << "1. Si" << endl;
                        cout << "2. No" << endl;

                        cin >> opcion;

                        if (opcion != 1 && opcion != 2){
                            cout << "Opcion invalida." << endl;
                        }

                    } while (opcion != 1 && opcion != 2);

                } while (opcion == 1);

                break;
            }

            case 2:{

                cout << "\n-- Busqueda por fecha --" << endl;

                string fechaBuscar;

                do {

                    cout << "Ingrese fecha (DD/MM/AAAA): ";
                    cin >> fechaBuscar;

                    if (!actividad::validarFormatoFecha(fechaBuscar)){
                        cout << "Formato invalido." << endl;
                    }

                } while (!actividad::validarFormatoFecha(fechaBuscar));

                list<actividad> actividadesEncontradas =
                buscarActividadFecha(fechaBuscar, listaActividades);

                if (actividadesEncontradas.size() > 0){

                    cout << "\nActividades encontradas:\n" << endl;

                    for (actividad act : actividadesEncontradas){
                        act.imprimirActividad();
                    }

                } else {

                    cout << "No existen actividades para esa fecha." << endl;
                }

                break;
            }

            case 3:{

                cout << "\n-- Modificar actividad --" << endl;

                if (listaActividades.empty()){

                    cout << "No hay actividades registradas." << endl;
                    break;
                }

                listarActividades(listaActividades);

                int numeroModificar;

                cout << "Ingrese numero de actividad a modificar: ";
                cin >> numeroModificar;

                if (numeroModificar < 1 || numeroModificar > listaActividades.size()){

                    cout << "Numero invalido." << endl;
                    break;
                }

                int contador = 1;

                for (actividad& act : listaActividades){

                    if (contador == numeroModificar){

                        string nuevaDescripcion;

                        cin.ignore();

                        cout << "Nueva descripcion: ";
                        getline(cin, nuevaDescripcion);

                        act.setDescripcion(nuevaDescripcion);

                        cout << "Actividad modificada correctamente." << endl;

                        break;
                    }

                    contador++;
                }

                break;
            }

            case 4:{

                cout << "\n-- Eliminar actividad --" << endl;

                if (listaActividades.empty()){

                    cout << "No hay actividades registradas." << endl;
                    break;
                }

                listarActividades(listaActividades);

                int numeroEliminar;

                cout << "Ingrese numero de actividad a eliminar: ";
                cin >> numeroEliminar;

                if (numeroEliminar < 1 || numeroEliminar > listaActividades.size()){

                    cout << "Numero invalido." << endl;
                    break;
                }

                int contador = 1;

                for (auto it = listaActividades.begin(); it != listaActividades.end(); it++){

                    if (contador == numeroEliminar){

                        listaActividades.erase(it);

                        cout << "Actividad eliminada correctamente." << endl;

                        break;
                    }

                    contador++;
                }

                break;
            }

            case 5:{

                cout << "Saliendo del programa..." << endl;
                return 0;
            }

            default:{
                cout << "Opcion invalida." << endl;
            }
        }
    }

    return 0;
}