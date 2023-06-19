#include <stdio.h>
#include <string.h>
#include <iostream>
#define BUF_SIZE 1024

using namespace std;

int suma_indirecta(void)
{
    int buf[BUF_SIZE];
    int a = 4;
    int i, v;
    memset(buf, 0, sizeof(buf));
    int b = 4;
    while (cin >> i >> v)
    { // Leo el índice y el valor
        if (i == -1)
            break; // Un índice -1 significa que tengo que terminar.
        if (i < BUF_SIZE){
            buf[i] = v; // Guardo el valor en el buffer
            cout << "buf[i] vale " << buf[i] << " b vale " << b << " a vale " << a << endl;
        }
    }
    // Calculo la suma de los valores
    v = 0;
    for (i = 0; i < BUF_SIZE; i++)
        v += buf[i];
    return v;
}

int main()
{

    cout << suma_indirecta() << endl;

    return 0;
}