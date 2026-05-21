#include <string>
#include <iostream>

using namespace std;

int main(){
    double num1;
    double num2;
    int opcion;

    while (true){
        cout << "\n==== CALCULADORA SENCILLA | LABORATORIO 09 PARTE 05 ====" << endl;
        cout << "Ingrese la operacion a realizar: " << endl;
        cout << "1: Suma\n2: Resta\n3: Division\n4: Multiplicacion.\n0:Salir" << endl;
        cin >> opcion;

        if (opcion!=0){
            cout << "Ingrese el primer numero con el que va a operar: " << endl;
            cin >> num1;
            cout << "Ingrese el segundo numero con el que va a operar: " << endl;
            cin >> num2;

            switch (opcion){
                case 1:
                    cout << "El resultado es: " << num1+num2 << endl;
                    break;
                case 2:
                    cout << "El resultado es: " << num1-num2 << endl;
                    break;
                case 3:
                    cout << "El resultado es: " << num1/num2 << endl;
                    break;
                case 4:
                    cout << "El resultado es: " << num1*num2 << endl;
                    break;
                default:
                    cout << "Operación invalida." << endl;
                    break;
            }

            cout<<"\nDesea realizar otra operacion\n1: Si\n0: No"<<endl;
            cin >> opcion;
            switch (opcion){
            case 0:
                return 0;
            case 1:
                cout << "\n==== FIN DEL CALCULO ====\n" << endl;
                break;
            default:
                cout<<"Opcion invalida, intente nuevamente...";
                break;
            }
        }
        else {
            return 0;
        }
    }
    

}