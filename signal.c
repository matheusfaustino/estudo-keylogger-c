#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/**
 * funcao executada como callback de um sinal do sistema. Nesse caso de exit do programa
 * @param ID sinal
 */
void signalDie(int signum)
{
	printf("Sinal pego #%d, saindo...\n", signum);

	// tem o exit pq senao o programa nao finaliza msm com o ctrl+c
	exit(1);
}

/**
 * nao descobri como chamar isso ainda
 * @param
 */
void signalTerm(int signum)
{
	printf("Terminar requisicao: #%d\n", signum);
	exit(1);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, signalDie);
	signal(SIGTERM, signalTerm);

	while (1) {
		printf("Going to sleep for a second... \n");
		sleep(1);
	}
	return 0;
}