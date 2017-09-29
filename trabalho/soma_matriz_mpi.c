// escreva um programa que calcula a soma de 
// todos os elementos de uma matriz A(n x n). 
// Considere que o processo 0 recebe como entrada da linha de comando
// o valor n, gera os elementos da matriz de modo randomico e distribui
// a computacao da soma pelos processos

// Na saida, o processo 0 deve imprimir o valor da soma de todos os elementos

#include <stdio.h>
#include <math.h> //random
#include <stdlib.h> //atoi, exit
#include <sys/time.h> //cronometro
#include <mpi.h>

int main(int argc, char const *argv[]) {
	int tag=0, i=0, j=0, num_processos, ID_processo;
	double sum=0, local_sum=0;

	MPI_Status status;	

	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &num_processos);

	MPI_Comm_rank(MPI_COMM_WORLD, &ID_processo);

	if (argv[1] == NULL || atoi(argv[1]) < 1) {
		printf("ERRO, insira o valor de 'n': \n");
		exit(1);
	}
	
	//tamanho da matriz a ser gerada
	int n = atoi(argv[1]);
	
	//matriz global
	double matriz[n][n];

	// preenchimento da matriz
	//vai ser usado no random do preenchimento
	// srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	if (ID_processo == 0) {
		for (i=0; i<n; i++) {
			for (j = 0; j < n; j++)	{
				// matriz[i][j] = random();
				matriz[i][j] = 1.0;  
			}
		}

		//envio de dados
		for (i = 1; i < num_processos; ++i) {
			MPI_Send(&matriz, n*n, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&matriz, n*n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//processamento de dados

	//caso 0: n == word_size
	if (n == num_processos) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; ++j) {
				if (ID_processo == i) {
					local_sum+= matriz[i][j];
				}
			}
		}
	}

	//caso 1 e caso 2:
		//1:	n > word_size : algum processo vai ficar sobrecarregado
		//2:	n < word_size : algum processo vai ficar ocioso
	// o ideal seria distribuir proporcionalmente para cada processo,
	// mas isso é uma refinação desnecessária quadno o objetivo é entender
	// o uso da biblioteca
	else {
		int counter = 0;
		for (i = 0; i < n; i++) {
			// printf("counter==%d\n",counter);
			for (j = 0; j < n; ++j) {
				if (ID_processo == counter) {
					local_sum+= matriz[i][j];
				}
			}
			if (counter < ID_processo)
				counter++;
			else
				counter = num_processos-1;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// printf("local_sum=%f, ID_processo=%d\n", local_sum,ID_processo);

	//coleta de dados
	MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (ID_processo == 0) {
		printf("sum=%f \n", sum);
	}
	
	MPI_Finalize();
	return 0;
}