#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex recursoA;
std::mutex recursoB;

void proceso1() {
    std::cout << "P1 intenta tomar Recurso A\n";
    recursoA.lock();
    std::cout << "P1 tomo Recurso A\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "P1 intenta tomar Recurso B\n";
    recursoB.lock();
    std::cout << "P1 tomo Recurso B\n";

    std::cout << "P1 trabajando...\n";

    recursoB.unlock();
    recursoA.unlock();
}

void proceso2() {
    std::cout << "P2 intenta tomar Recurso A\n";
    recursoA.lock();
    std::cout << "P2 tomo Recurso A\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "P2 intenta tomar Recurso B\n";
    recursoB.lock();
    std::cout << "P2 tomo Recurso B\n";

    std::cout << "P2 trabajando...\n";

    recursoB.unlock();
    recursoA.unlock();
}

int main() {
    std::thread t1(proceso1);
    std::thread t2(proceso2);

    t1.join();
    t2.join();

    std::cout << "Fin del programa\n";
    return 0;
}

/*
En la primera versión se produce una situación de deadlock porque cada proceso adquiere un recurso diferente 
y luego intenta obtener el recurso que posee el otro proceso. 
Como ninguno libera el recurso que tiene, ambos quedan esperando indefinidamente. 
En la segunda versión el problema se evita imponiendo un orden fijo de adquisición de recursos: ambos procesos solicitan primero el recurso A y luego el recurso B. 
De esta manera se elimina la espera circular, una de las condiciones necesarias para que ocurra un deadlock, permitiendo que los procesos finalicen correctamente.
*/