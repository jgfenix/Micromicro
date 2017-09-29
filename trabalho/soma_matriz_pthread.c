#include <stdio.h>
#include <math.h> //random
#include <stdlib.h> //atoi, exit
#include <sys/time.h> //cronometro
#include <pthread.h>


#ifndef PROCESSOS_E_THREADS
#define PROCESSOS_E_THREADS 4
#endif

struct matriz_e_tamanho {
	double** m;
	long n;
	double sum;
};

// TODO(se der tempo): Thread parent so esta gerando e distribuindo a matriz, nao esta processando ela

//controle do numero de threads childs
long tID = 0; //thread 0 é a thread que gera a matriz
pthread_mutex_t mutex;


void *ProcessaMatriz(void *m_n) {
	pthread_mutex_lock(&mutex);
		long local_tID = tID;
		tID++;
	pthread_mutex_unlock(&mutex);

	double local_sum=0;
	
	struct matriz_e_tamanho *mat_n = (struct matriz_e_tamanho *)m_n;

	int i,j;

	//processamento de dados

	//caso 0: size(matriz) == size(threads)
	if (mat_n->n == PROCESSOS_E_THREADS) {
		for (i = 0; i < mat_n->n; i++) {
			for (j = 0; j < mat_n->n; ++j) {
				if (local_tID == i) {
					local_sum+= mat_n->m[i][j];
				}
			}
		}
	}

	//caso 1 e caso 2:
		//1:	size(matriz) > size(threads) : algum processo vai ficar sobrecarregado
		//2:	size(matriz) < size(threads) : algum processo vai ficar ocioso
	// o ideal seria distribuir proporcionalmente para cada processo,
	// mas isso é uma refinação desnecessária quando o objetivo é entender
	// o uso da biblioteca
	else {
		int counter = 0;
		for (i = 0; i < mat_n->n; i++) {
			// printf("counter==%d\n",counter);
			for (j = 0; j < mat_n->n; ++j) {
				if (local_tID == counter) {
					local_sum+= mat_n->m[i][j];
				}
			}
			if (counter < local_tID)
				counter++;
			else
				counter = PROCESSOS_E_THREADS-1;
		}
	}
	// printf("Thread: %ld| LOCAL_SUM==%f\n", local_tID, local_sum);
	// pthread_exit(NULL);
	pthread_mutex_lock(&mutex);
		mat_n->sum+=local_sum;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void *CriaMatriz(void *tamanhoMatriz) {
	long n ;
	n = (long)tamanhoMatriz;
	int i, j;
 
	double **matriz = (double**)malloc(n * sizeof(double*)); //Aloca um Vetor de Ponteiros
	//vai ser usado no random do preenchimento
	// srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	for (i = 0; i < n; i++){ //Percorre as linhas do Vetor de Ponteiros
		matriz[i] = (double*) malloc(n * sizeof(double)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
		for (j = 0; j < n; j++){ //Percorre o Vetor de Inteiros atual.
			// matriz[i][j] = random();
			matriz[i][j] = 1.0;  
		}
	}
	
	//struct para passar a matriz e o tamanho dela
	struct matriz_e_tamanho* mat_n = malloc( sizeof( struct matriz_e_tamanho ));
	mat_n->m = matriz;
	mat_n->n = n;
	mat_n->sum = 0;

	//criacao de novas threads
	int sub_threads = PROCESSOS_E_THREADS;//-1; //-1 pois a 0 já foi criada

	pthread_t thread[sub_threads];
	void *status;
	//inicia o mutex
	pthread_mutex_init(&mutex, NULL);
	
	int t_child =0;
	
	for(t_child =0; t_child < sub_threads; t_child++) {
		pthread_create(&thread[t_child], NULL, ProcessaMatriz, mat_n); //enviar a matriz
	}
	
	//espera todas as threads acabarem
	for(t_child = 0; t_child < sub_threads; t_child++) {
		pthread_join(thread[t_child], &status);
	}

	printf("sum==%f\n",mat_n->sum );
	free(matriz);
	pthread_exit(NULL);
	return NULL;
}


int main (int argc, char *argv[])
{
	//verificacao de passagem de parametros	
	if (argv[1] == NULL || atoi(argv[1]) < 1) {
		printf("ERRO, insira o valor de 'n': \n");
		exit(1);
	}

	long n = (long)atoi(argv[1]);

	pthread_t thread0;
	void *status;
	
	//thread 0 que gera a matriz
	//inicio da contagem de tempo
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);

	pthread_create(&thread0, NULL, CriaMatriz, (void *)n);

	pthread_join(thread0, &status);

	gettimeofday(&tv2, NULL);
	printf ("Total time = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));
	// printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
}