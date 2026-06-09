#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

class Estudiante {
    private:
        string nombre;

    public:

        Estudiante(string nombre){
            this->nombre = nombre;
        }

        void servirseCafe(){
            cout << this->nombre << " se acaba de servirse cafe. " << endl;
        }

        void entrarCocina(){
            cout << this->nombre << " entro a la cocina. " << endl;
        }

        void salirCocina(){
            cout << this->nombre << " salio de la cocina. " << endl;
        }

        string getNombre(){
            return this->nombre;
        }
};

void usarCafetera(Estudiante estudiante, mutex& cafetera)
{
    for(int i = 0; i < 2; i++)
    {
        //esto es para esperar un tiempo aleatorio para servirse café
        this_thread::sleep_for(
            chrono::seconds(rand() % 3 + 2)
        );

        cout << estudiante.getNombre() << " quiere usar la cafetera." << endl;

        {
            lock_guard<mutex> lock(cafetera);

            estudiante.entrarCocina();

            estudiante.servirseCafe();

            this_thread::sleep_for(
                chrono::seconds(rand() % 3 + 1)
            );

            estudiante.salirCocina();
        }
    }
}

int main(){
    srand(time(NULL));
    mutex cafetera;

    vector<Estudiante> adictosAlCafe;

    adictosAlCafe.push_back(Estudiante("Emmanuel"));
    adictosAlCafe.push_back(Estudiante("Nico"));
    adictosAlCafe.push_back(Estudiante("Lucas"));
    adictosAlCafe.push_back(Estudiante("Nacho"));
    
    vector<thread> hilos;

    for(auto& adicto : adictosAlCafe){
        hilos.push_back(thread(usarCafetera,adicto,ref(cafetera)));
    }

    for (auto& hilo : hilos){
        hilo.join();
    }

    return 0;
}