/*
Simulación de solicitudes de I/O usando productor-consumidor.
3 procesos generan solicitudes y un driver las atiende.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

queue<string> colaIO;
mutex mtx;
condition_variable cv;

bool finalizar = false;

// Productor (proceso)
void proceso(int id)
{
    for (int i = 1; i <= 5; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(500));

        string solicitud = "Proceso " + to_string(id) + " - Solicitud " + to_string(i);

        {
            lock_guard<mutex> lock(mtx); //Si el mutex ya esta bloqueado, el proceso se bloquea hasta que pueda adquirirlo

            colaIO.push(solicitud);

            cout << "[Proceso " << id << "] Genero la solicitud " << i << endl;
        }

        cv.notify_one();
    }
}

// Consumidor (driver)
void driver()
{
    ofstream log("log.txt");

    while (true)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [] {
            return !colaIO.empty() || finalizar;
        });

        if (colaIO.empty() && finalizar)
        {
            break;
        }

        string solicitud = colaIO.front();
        colaIO.pop();

        lock.unlock();

        {
            lock_guard<mutex> salida(mtx);
            cout << "   [DRIVER] Atendiendo: "
                 << solicitud << endl;
        }

        this_thread::sleep_for(chrono::seconds(1));

        log << solicitud << " atendida correctamente" << endl;
    }

    log.close();
}

int main()
{
    thread p1(proceso, 1);
    thread p2(proceso, 2);
    thread p3(proceso, 3);

    thread dispositivo(driver);

    p1.join();
    p2.join();
    p3.join();

    {
        lock_guard<mutex> lock(mtx);
        finalizar = true;
    }

    cv.notify_one();

    dispositivo.join();

    cout << "\nFin de la simulacion. Ver log.txt" << endl;

    return 0;
}