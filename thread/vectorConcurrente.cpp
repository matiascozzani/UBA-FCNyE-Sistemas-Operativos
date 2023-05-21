#include<unistd.h>
#include<iostream>
#include<vector>
#include<thread>
#include<semaphore.h>
#include<numeric>
#include<atomic>
using namespace std;

template<typename T>
class concurrentVector : public vector<T> {
    public:
        concurrentVector(int t, int tCount) : vector<T>(t){
            this->thread_count = tCount;
            this->n = t;
            this->posicion = 0;
        }
        int operator*(concurrentVector<T> v){
            for(int i = 0; i < this->thread_count; i++){

                threads.emplace_back(this->calcular, i, v);

            }
            for(auto &t: threads){
                t.join();
            }

            return sumaAcumulada;
        }
    private:
        int sumaAcumulada = 0;
        std::atomic_int posicion;
        int thread_count;
        int n;
        vector<thread> threads;
        void calcular(int j, concurrentVector<T> v){
            while(posicion < 100){
                this->sumaAcumulada += this->at(posicion) * v[posicion];
                posicion++;
                this_thread::sleep_for(100ms*(j+1));
            }
        }
};

int main(int argc, char* argv[]){
    const int N = 100;
    const int THREAD_COUNT = 5;
    concurrentVector<int> v1(N, THREAD_COUNT);
    concurrentVector<int> v2(N, THREAD_COUNT);
    iota(v1.begin(), v1.end(), 1);
    iota(v2.begin(), v2.end(), 101);
   
    cout << "El resultado es " << v1 * v2 << endl;

    return 0;
}