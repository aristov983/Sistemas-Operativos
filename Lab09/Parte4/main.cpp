#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    string nombre;
    int nota;

    cout << "Ingrese su nombre: " << endl;
    cin >> nombre;
    cout << "Ingrese su nota final: " << endl;
    cin >> nota;

    if (nota<0 || nota>100)
    {
        cout << "Ingreso una nota fuera de rango." << endl;
    }
    else
    {
        cout << "==== RESULTADOS ====" << endl;
        if (nota<=24)
        {
            cout << "No aprueba.";
        }
        else
        {
            if (nota <= 74)
            {
                cout << "Aprueba.";
            }
            else
            {
                cout << "Excelente";
            }
        }
    }
}