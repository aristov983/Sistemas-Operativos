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

    recursoB.unlock();
    recursoA.unlock();
}

void proceso2() {
    std::cout << "P2 intenta tomar Recurso B\n";
    recursoB.lock();
    std::cout << "P2 tomo Recurso B\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "P2 intenta tomar Recurso A\n";
    recursoA.lock();
    std::cout << "P2 tomo Recurso A\n";

    recursoA.unlock();
    recursoB.unlock();
}

int main() {
    std::thread t1(proceso1);
    std::thread t2(proceso2);

    t1.join();
    t2.join();

    std::cout << "Fin del programa\n";
    return 0;
}