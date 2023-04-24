#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
pid_t hijo;
int pipes[2] = {3,4};
int pid;

void sigchldHandler(int sig){
    char* msg = "Te voy a buscar en la oscuridad.\n";
    write(1, msg, 34);
    close(pipes[0]);
    exit(0);
}

void sigchldHandlerHijo(int sig){

}

void sigintHandler(int sig){
    char* input;
    read(pipes[0], input, 3);
    char* msg1 = "Mirá vos. El significado de la vida es 42.\n";
    write(1, msg1, 44);
    char* bang = "¡Bang Bang, estás liquidado!\n";
    write(1, bang, 32);
    kill(hijo, SIGHUP);
    signal(SIGCHLD, sigchldHandler);
    sleep(10);
    
}
void sigintHandlerHijo(int sig){
    char* msg = "Dejame pensarlo...\n";
    write(1, msg, 19);
    signal(SIGCHLD, sigchldHandlerHijo);
    sleep(5);
    char* msg2 = "Ya sé el significado de la vida\n";
    write(1, msg2, 33);
    write(pipes[1], "42", 2);
    kill(pid, SIGINT);
}
void sigupHandler(int sig){
    char* msg = "Me voy a mirar crecer las flores\n";
    write(1, msg, 33);
    close(pipes[1]);
    exit(0);
}

int main(int argc, char* argv[]){

    if(pipe(pipes)==-1){
        perror("ERROR");
    }

    hijo = fork();
    if(hijo==-1) perror("ERROR");
    if(hijo==0){
        close(pipes[0]);
        pid = getppid();
        signal(SIGINT, sigintHandlerHijo);
        signal(SIGHUP, sigupHandler);
        while(1){}
    }else{
        close(pipes[1]);
        signal(SIGINT, sigintHandler);
        signal(SIGCHLD, sigchldHandler);
        sleep(1);
        char* msg = "¿Cuál es el significado de la vida?\n";
        write(1, msg, 38);
        kill(hijo, SIGINT);
        while(1){

        }
    }

}