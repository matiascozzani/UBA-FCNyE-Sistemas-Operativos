#include<unistd.h>
#include<iostream>
#include<vector>
#include<thread>
#include<semaphore.h>
#include<numeric>
#include<atomic>
using namespace std;


vector<int> v1(100);
vector<int> v2(100);
vector<int> v3(100);
int sumaAcumulada = 0;
vector<int> cantidadOperaciones(5);
atomic<int> posicion(0);
const int MSG_COUNT = 5;
void calcular(int i){
    while(posicion < 100){
        sumaAcumulada += v1[posicion]*v2[posicion];
        posicion++;
        cantidadOperaciones[i]++;
        this_thread::sleep_for(100ms*(i+1));
    }
}


int main(int argc, char* argv[]){
    vector<thread> threads;

    iota(v1.begin(), v1.end(), 1);
    iota(v2.begin(), v2.end(), 101);

    for(int i = 0; i < 5; i++){
        threads.emplace_back(calcular, i);
    }

    for(auto &t: threads){
        t.join();
    }

    cout << "Vector 1: " << endl;
    for(int i = 0; i < 100; i++){
        cout << v1[i] << " ";
    }
    cout << endl;
    cout << "Vector 2: " << endl;
    for(int i = 0; i < 100; i++){
        cout << v2[i] << " ";
    }
    cout << endl;
    cout << "Suma Acumulada: " << sumaAcumulada << endl;
    cout << "Cantidad: " << endl;
    for(int i = 0; i < 5; i++){
        cout << cantidadOperaciones[i] << " ";
    }

    return 0;
}