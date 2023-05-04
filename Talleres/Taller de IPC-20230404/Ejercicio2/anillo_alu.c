#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>
#include "constants.h"

enum {READ, WRITE};

int first_run_done = 0;
int n;
int generate_random_number(){
	return (rand() % 50);
}

void ejecutarStart(int i, int pipes[][2]){

	int buffer;
	read(pipes[0], &buffer, sizeof(buffer));

	int random;
	do{
		random = generate_random_number();
	}while(random < buffer);

	int next = (i+n+1)%n;

	for(int j = 0; j <= n; j++){
		if(j == i)
			close(pipes[j][1]);
		else if(j == next)
			close(pipes[j][0]);
		else if(j == n)
			close(pipes[n][0]);
		else{
			close(pipes[j][0]);
			close(pipes[j][1]);
		}

	}

	int value;
	do{
		read(pipes[i][0], &value, sizeof(value));
		value++;
		write(pipes[next][1], &value, sizeof(value));
	}while(value <= random);
	
	write(pipes[n][1], &value, sizeof(value));
	close(pipes[n][1]);
	exit(0);

}

void ejecutarHijo(int i, int pipes[][2]){
	int next = (i+n+1)%n;
	for(int j = 0; j <= n; j++){
		if(j == i)
			close(pipes[i][1]);
		else if(j == next)
			close(pipes[j][0]);
		else{
			close(pipes[j][0]);
			close(pipes[j][1]);
		}
	}
	int value;
	while(1){
		read(pipes[i][0], &value, sizeof(value));
		value++;
		write(pipes[next][1], &value, sizeof(value));
	}
}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, start, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);


    /* COMPLETAR */
	int pids[n];
	int pipes[n+1][2];
	for(int i = 0; i <= n; i++){
		pipe(pipes[i]);		
	}
	for(int i = 0; i < n; i++){
		pids[i] = fork();
		if(pids[i]==0 && i == start){
			ejecutarStart(i, pipes);
		}
		else if(pids[i] == 0)
			ejecutarHijo(i, pipes);
	}
	for(int i = 0; i <= n; i++){
		if(i == start){
			write(pipes[start][1], &buffer, sizeof(buffer));
			close(pipes[start][1]);
			close(pipes[start][0]);
		}else if(i == n){
			close(pipes[n][1]);
		}else{
		close(pipes[i][0]);
		close(pipes[i][1]);
		}
	}

	waitpid(pids[start], NULL, 0);
	int finalValue;
	read(pipes[n][0], &finalValue, sizeof(finalValue));
	printf("Valor final: %d\n", finalValue);
	
	return 0;

	
}