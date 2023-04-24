#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, const char *argv[])
{
    int value = 0;
    int pipes_padre_hijo[2];
    int pipes_hijo_padre[2];

    if (pipe(pipes_padre_hijo)==-1 || pipe(pipes_hijo_padre)==-1)
        perror("Error en pipe");

    int pid = fork();
    if (pid == -1)
        perror("Error en fork");
    if (pid == 0)
    {
        while(value < 50){
            read(pipes_padre_hijo[0], &value, sizeof(value));
            value++;
            write(pipes_hijo_padre[1], &value, sizeof(value));
            printf("Escribi %d\n", value);
        }
        exit(0);       
    }else{
        write(pipes_padre_hijo[1], &value, sizeof(value));
        while(value < 50);
        {
            read(pipes_hijo_padre[0], &value, sizeof(value));
            value++;
            write(pipes_padre_hijo[1], &value, sizeof(value));

        }
        return 0;
    }
}
