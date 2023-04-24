#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, const char *argv[])
{
    int value = 0;
    int pipes[2];
    int pipes_hijos[2];

    if (pipe(pipes) == -1 || pipe(pipes_hijos) == -1)
        perror("Error en pipe");

    int pid = fork();
    if (pid == -1)
        perror("Error en fork");
    if (pid == 0)
    {
        close(pipes[1]);
        close(pipes_hijos[0]);
        while (value < 50)
        {
            read(pipes[0], &value, sizeof(value));
            value++;
            write(pipes_hijos[1], &value, sizeof(value));
            printf("Hijo1 escribio %d\n", value);
        }
        exit(0);
    }

    int pid2 = fork();
    if(pid2==-1) perror("Error en 2 pipe");
    if (pid2 == 0)
    {
        close(pipes[0]);
        close(pipes_hijos[1]);
        while (value < 50)
        {
            read(pipes_hijos[0], &value, sizeof(value));
            value++;
            write(pipes[1], &value, sizeof(value));
            printf("Hijo2 escribio %d\n", value);
        }
        exit(0);
    }

    write(pipes[1], &value, sizeof(value));
    while (value < 50)
    {
        read(pipes[0], &value, sizeof(value));
        value++;
        write(pipes[1], &value, sizeof(value));
        printf("PADRE ESCRIBE %d\n", value);
    }
    exit(0);
    return 0;
}
