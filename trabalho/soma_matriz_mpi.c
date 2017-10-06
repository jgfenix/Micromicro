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
	int i=0, j=0, num_processos, ID_processo;
	double sum=0, local_sum=0;
	

	if (argv[1] == NULL || atoi(argv[1]) < 1) {
		printf("ERRO, insira o valor de 'n': \n");
		exit(1);
	}
	
	//tamanho da matriz a ser gerada
	int n = atoi(argv[1]);

	MPI_Status status;	

	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &num_processos);

	MPI_Comm_rank(MPI_COMM_WORLD, &ID_processo);

	// Envio do valor de n para todos os processos
	// MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (  n%num_processos !=0 ) {
		printf("ERROR: matriz indivisível por %d processos \n", num_processos);
		MPI_Finalize();
		exit(1);
	}
	
	double *vetor_local = (double*) calloc(n, sizeof(double));

	//matriz global
	double matriz[n][n];

	// preenchimento da matriz
	//vai ser usado no random do preenchimento
	struct timeval tm;
	gettimeofday(&tm, NULL);
	srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	if (ID_processo == 0) {
		for (i=0; i<n; i++) {
			for (j = 0; j < n; j++)	{
				matriz[i][j] = rand() % 9;
				// matriz[i][j] = 1.0;  
			}
		}

		for (i=0; i<n; i++) {
			for (j = 0; j < n; j++)	{
				printf("%2.0f ", matriz[i][j]);
				
			}
			printf("\n");
		}

		//envio de dados
		for (i = 1; i < num_processos; ++i) {
			MPI_Send( &(matriz[i][(0)]), n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD) ;
		}
		
		//carrega a parcela do processo 0
		for (i = 0; i < n; ++i) {
			(vetor_local[i]) = (matriz[0][i]);
		}
	}
	else
		MPI_Recv(vetor_local, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status) ;

	MPI_Barrier(MPI_COMM_WORLD);

	//processamento de dados

	printf("\n");
	for (i = 0; i < num_processos; i++) {
		if (i == ID_processo) {
			local_sum = 0;
			printf("Proc: %d ->", i );
			for ( j = 0; j < n; ++j) {
				printf("%.0f  ",vetor_local[j] );
				local_sum+= vetor_local[j];
			}
			printf("Proc: %d -> local_sum=%.0f\n", ID_processo, local_sum);
		}
	}	

	MPI_Barrier(MPI_COMM_WORLD);

	//coleta de dados
	MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (ID_processo == 0) {
		printf("sum=%f \n", sum);
	}
	
	MPI_Finalize();
	return 0;
}