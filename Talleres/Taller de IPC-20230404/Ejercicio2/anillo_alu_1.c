#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>
#include "constants.h"
int first_run_done = 0;
int n;
int generate_random_number(){
	return (rand() % 50);
}

void ejecutarPrimerHijo(int i, int pipes[][2], int buffer){
	for(int j = 0; j <= n; j++){
		if(j == i || j == i-1 || j == n) continue;
		close(pipes[j][0]);
		close(pipes[j][1]);
	}

	close(pipes[i][0]);
	close(pipes[n][0]);
	int anterior = i-1 <= 0 ? 0 : i-1;
	close(pipes[anterior][1]);

	int random = 0;
	if(!first_run_done){
		random = generate_random_number() * buffer;
		printf("RANDOM VALE %d\n", random);
		write(pipes[i][1], &buffer, sizeof(buffer));
		printf("SOY START, ESCRIBÍ %d EN EL PIPE\n", buffer);
		first_run_done = 1;
	}
	int aux;
	do
	{
		read(pipes[anterior][0], &aux, sizeof(aux));
		printf("SOY PROCESO %d LEÍ AUX %d DEL PIPE\n", i, aux);
		do
		{
			read(pipes[anterior][0], &buffer, sizeof(buffer));
			printf("SOY PROCESO %d LEI BUFFER %d DEL PIPE\n", i, buffer);
		} while (aux == buffer || aux == 0);

		buffer++;
		write(pipes[i][1], &buffer, sizeof(buffer));
		printf("SOY PROCESO %d, INICIO NUEVA VUELTA CON %d\n", i, buffer);
	}while(buffer < random);

	write(pipes[n][1], &buffer, sizeof(buffer));
	printf("SOY PROCESO %d, ESCRIBÍ %d A PAPA\n", i, buffer);
	exit(0);
}

void ejecutarHijo(int i, int pipes[][2]){
	for(int j = 0; j <= n; j++){
		if(j == i || j == i-1) continue;
		close(pipes[j][0]);
		close(pipes[j][1]);
	}

	int anterior = i - 1 <= 0 ? 0 : i-1;
	close(pipes[anterior][1]);
	close(pipes[i][0]);
	int buffer;
	int aux;
	do{
		read(pipes[anterior][0], &aux, sizeof(aux) != sizeof(aux));
		printf("SOY proceso %d LEÍ AUX %d DEL PIPE\n", i, aux);
		do{
			read(pipes[anterior][0], &buffer, sizeof(buffer));
			printf("SOY proceso %d LEÍ BUFFER %d DEL PIPE\n", i, buffer);
		}
		while(aux == buffer || aux == 0);
		buffer++;
		write(pipes[i][1], &buffer, sizeof(buffer));
		printf("SOY proceso %d ESCRIBI %d EN PIPE\n", i, buffer);
	}while(1);
}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, p, start, buffer;
	p = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);
	n = p;

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
	int pipes[n+1][2];
	for(int i = 0; i <= n; i++){
		pipe(pipes[i]);
	}

    /* COMPLETAR */
	int pids[n];
    for(int i = 0; i < n; i++){
		pids[i] = fork();
		if(pids[i]==-1) perror("ERROR AL FORK");
		if(pids[i]==0){
			if(i == start){
				ejecutarPrimerHijo(i, pipes, buffer);
			}
			else{
				ejecutarHijo(i, pipes);
			}
		}
	}
	waitpid(pids[start], NULL, 0);
	for(int i = 0; i < n; i++){
		if(i == start) continue;
		kill(pids[i], SIGTERM);
	}
	int finalNumber = 0;
	read(pipes[n][0], &finalNumber, sizeof(finalNumber));
	printf("Número final: %d", finalNumber);
	exit(0);
	return 0;
}
