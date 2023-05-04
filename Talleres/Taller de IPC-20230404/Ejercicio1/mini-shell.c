#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"
int countPipes = 0;
void ejecutar_hijo(int i, int pipes[][2], char **programa){
	if(i == 0){
		for(int j = 1; j < countPipes; j++){
			close(pipes[j][0]);
			close(pipes[j][1]);
		}
		close(pipes[i][0]);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	else if(i == countPipes){
		for(int j = 0; j < countPipes; j++){
			if(j == i-1) continue;
			close(pipes[j][0]);
			close(pipes[j][1]);
		}
		dup2(pipes[i-1][0], STDIN_FILENO);
		close(pipes[i-1][1]);
	}
	else{
		for(int j = 0; j < countPipes; j++){
			if(j==i || j == i-1) continue;
			close(pipes[j][0]);
			close(pipes[j][1]);
		}
		dup2(pipes[i-1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
		close(pipes[i-1][1]);
		close(pipes[i][0]);
	}
	execvp(programa[0], programa);
}

static int run(char ***progs, size_t count)
{	
	int r, status;
	countPipes = count-1;
	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);
	//TODO: Pensar cuantos procesos necesito = count procesos.
	//TODO: Pensar cuantos pipes necesito = count/2.

	int pipes[countPipes][2];
	for(int i = 0; i < countPipes; i++)
		pipe(pipes[i]);

	for(int i = 0; i < count; i++){
		children[i] = fork();
		if(children[i]==-1){
			perror("ERROR AL FORKEAR");
			exit(1);
		}
		if(children[i] == 0){
			ejecutar_hijo(i, pipes, (progs)[i]);
		}
	}

	for(int i = 0; i < countPipes; i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	return r;
}


int main(int argc, char **argv)
{
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
