#include<thread>
#include<iostream>
#include<chrono>
#include<unistd.h>
#include<vector>
#include<stdio.h>
#include<semaphore>
using namespace std;
mutex mtx;
binary_semaphore barrera_a(1);
binary_semaphore barrera_b(1);

//chicos disculpen, me surgió un incoveniente. dejo el liveshare abierto.
// dale np

//no segui con los ej porq me perdi al principio de la clase, y se esta crasheando cosas, en un rato intento, dentro de todo entendi
void f1_a(){
    for (size_t i = 0; i < 10; i++)
    {
        cout<< "Ejecutando F1(A)"<<endl;
        this_thread::sleep_for(100ms);
    }
}
void f1_b(){
    for (size_t i = 0; i < 10; i++)
    {
        cout<< "Ejecutando F1(B)"<<endl;
        this_thread::sleep_for(200ms);
    }
}
void f2_a(){
    for (size_t i = 0; i < 10; i++)
    {
        cout<< "Ejecutando F2(A)"<<endl;
        this_thread::sleep_for(500ms);
    }
}
void f2_b(){
    for (size_t i = 0; i < 10; i++)
    {
        cout<< "Ejecutando F2(B)"<<endl;
        this_thread::sleep_for(10ms);
    }
}
void f1(){
    f1_a();
    barrera_b.release();
    barrera_a.acquire();
    //sem_post(barrera_b);
    //sem_wait(barrera_a);
    f1_b();
}
void f2(){
    f2_a();
    barrera_a.release();
    barrera_b.acquire();
    //sem_post(barrera_a);
    //sem_wait(barrera_b);
    f2_b();
}

/*void f(int i){
    //unique_lock<mutex> lk(mtx) permite lk.lock y lk.unlock explícitamente
    for(int j = 0; j < 5; j++){ //<- versión con lockGuard
        { //<- scope anonimo
        lock_guard<mutex> lk(mtx); //cuando cierra el scope se unlockea, el destructor del template hace el unlock del mutex pasado por parámetro.
        printf("Hola! Soy el thread %d\n", i);
        this_thread::sleep_for(100ms);
        printf("Chau! Soy el thread %d\n", i);
        }
        this_thread::sleep_for(200ms);
    }
    /*for(int j = 0; j < 5; j++){
        mtx.lock(); //<- versión con mutex;
        printf("Hola! Soy el thread %d\n", i);
        this_thread::sleep_for(100ms);
        printf("Chau! Soy el thread %d\n", i);
        mtx.unlock();

        this_thread::sleep_for(200ms);
    }
    return;
}*/

int main(int argc, char* argv[]){
    vector<thread> threads;
    for(int i = 0; i < 10; i++){
        threads.emplace_back(f, i);
    }
    
    for(auto &t: threads){
        t.join();
        //t.detach();
    }
    return 0;
}


