#include <iostream>
#include <thread>

using namespace std;

void imprimirLetra(char letra)
{
    cout << letra;
}

int main()
{
    for(int i = 0; i < 6; i++)
    {
        // Esto crea un hilo que imprime cada letra y cada letra tiene su propio hilo.
        thread t1(imprimirLetra, 'S');
        thread t2(imprimirLetra, 'A');
        thread t3(imprimirLetra, 'L');
        thread t4(imprimirLetra, 'T');
        thread t5(imprimirLetra, 'O');

        // Esto hace que el programa espere a que el hilo termine antes de continuar. Sin join(), el programa podría finalizar antes de que los hilos impriman las letras.
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();

        // Salto de línea
        cout << endl;
    }

    return 0;
}