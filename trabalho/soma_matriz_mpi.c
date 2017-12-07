// escreva um programa que calcula a soma de 
// todos os elementos de uma matriz A(n x n). 
// Considere que o processo 0 recebe como entrada da linha de comando
// o valor n, gera os elementos da matriz de modo randomico e distribui
// a computacao da soma pelos processos

// Na saida, o processo 0 deve imprimir o valor da soma de todos os elementos

#include <stdio.h>
#include <stdlib.h> //atoi, exit, random
#include <sys/time.h> //cronometro
#include <mpi.h>

struct timeval  tv1, tv2;

int main(int argc, char const *argv[]) {
	int i=0, j=0, num_processos=0, ID_processo=0;
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

	if (  n%num_processos !=0 ) {
		printf("ERROR: matriz indivisível por %d processos \n", num_processos);
		MPI_Finalize();
		exit(1);
	}
	
	double *vetor_local = (double*) calloc(n*n/num_processos, sizeof(double));

	// preenchimento da matriz
	struct timeval tm;
	gettimeofday(&tm, NULL);
	srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	if (ID_processo == 0) {
		//matriz global
		double matriz[n][n];
		double soma = 0.0;
		for (i=0; i<n; i++) {
			for (j = 0; j < n; j++)	{
				soma += matriz[i][j] = rand() % 9;
				// matriz[i][j] = 1.0;  
			}
		}

		printf("\n\nSOMA=%f\n",soma );

		//envio de dados
		for (i = 1; i < num_processos; i++) {
			MPI_Send( &(matriz[i*n/num_processos][(0)]), n*n/num_processos, MPI_DOUBLE, i, 0, MPI_COMM_WORLD) ;
		}
		//carrega a parcela do processo 0
		int j = 0;
		int k = 0;
		for (i = 0; i < n/num_processos; i++) { //1000 | 2 
			for (j = 0; j < n; j++) {
				(vetor_local[k]) = (matriz[i][j]);
				k++;
			}
		}
	}
	else
		MPI_Recv(vetor_local, n*n/num_processos, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status) ;

	//MPI_Barrier(MPI_COMM_WORLD);
	
	//inicio da contagem de tempo
	if (ID_processo == 0) {
		gettimeofday(&tv1, NULL);
	}

	//processamento de dados
	local_sum = 0;
	for ( j = 0; j < n*n/num_processos; j++) {
		local_sum+= vetor_local[j];
	}
	//printf("Proc: %d -> local_sum=%.0f\n", ID_processo, local_sum);

	//MPI_Barrier(MPI_COMM_WORLD);

	//coleta de dados
	MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (ID_processo == 0) {
		
		//fim da contagem de tempo
		
		gettimeofday(&tv2, NULL);
		printf ("Total time = %f seconds\n",
	         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	         (double) (tv2.tv_sec - tv1.tv_sec));

		printf("sum=%f \n", sum);
	}
	
	MPI_Finalize();
	return 0;
}