#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector<string> msg {"hola","MUNDO","UCU","Salto"};
    for (const string& word : msg)
    {
        cout << word << " "; //si quisieramos imprimir cada palabra por linea usamos << endl al final (hace un salto de linea)
    }
    cout << endl;
}