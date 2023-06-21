#include <stdio.h>
int main(){

    int len = 1;

    if(len--==0) {
        puts("DECREMENTO ANTES DE COMPARAR");
    }
    else {
        puts("DECREMENTO DESPUES");
        printf("%d", len);
    }

    return 0;

}