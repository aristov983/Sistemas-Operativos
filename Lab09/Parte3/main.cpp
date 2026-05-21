#include <iostream>
#include <string>

using namespace std;

int main()
{
    int edad;
    string nombre;
    cout << "Ingrese su nombre: ";
    cin >> nombre;
    cout << "Ingrese su edad: ";
    cin >> edad;

    cout << "==== RESULTADOS ====" << endl;
    cout << "Estimado " + nombre << endl;
    if (edad > 0 && edad<=17){
        cout << "Usted es menor de edad.";
    }
    else
    {
        if (edad>17)
        {
            cout << "Usted es mayor de edad.";
        }
        else
        {
            cout << "Usted ingreso una edad negativa.";
        }
    }
}