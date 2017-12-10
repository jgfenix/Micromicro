#include <stdio.h>
#include <stdlib.h> //atoi, exit, random
#include <sys/time.h> //cronometro
#include <pthread.h>

#ifndef PROCESSOS_E_THREADS
#define PROCESSOS_E_THREADS 2
#endif

struct timeval  tv1, tv2;
long n;
double **matriz;
pthread_mutex_t mutex;
double sum = 0.0;

void *ProcessaMatriz(void *t_ID) {
	int * t = (int *) t_ID;
	int t_child = * t;
	

	int i=0, //linhas
		j=0; //colunas
	
	double local_sum=0;
	if (t_child == 0) {
		//inicio da contagem de tempo
		gettimeofday(&tv1, NULL);
	}
	//processamento de dados
	for (i = (n/PROCESSOS_E_THREADS)*t_child; i < ((t_child+1)*(n/PROCESSOS_E_THREADS)) ; ++i) {
		for (j = 0; j < n; ++j)
		{
			local_sum += matriz[i][j];
			// printf("thread %ld | m[%d][%d]=%.0f\n",t_child,i,j, m[i][j] );
		}
	}
	//mutex para nao ter mais de uma thread acessando a mesma variavel comum a todas
	pthread_mutex_lock(&mutex);//bloqueia a regiao critica
		sum+= local_sum;
	pthread_mutex_unlock(&mutex);//desbloqueia a regiao critica

	if (t_child == 0) {
		gettimeofday(&tv2, NULL);
		printf ("Total time = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));

	}
	return NULL;
}


int main (int argc, char *argv[]) {
	//verificacao de passagem de parametros	
	if (argv[1] == NULL || atoi(argv[1]) < 1) {
		printf("ERRO, insira o valor de 'n': \n");
		exit(1);
	}

	n = (long)atoi(argv[1]);

	double confere=0; //variavel so pra conferir se as threads estao somando corretamente
	int i, j;
 
	matriz = (double**)malloc(n * sizeof(double*)); //Aloca um Vetor de Ponteiros
	//vai ser usado no random do preenchimento
	struct timeval tm;
	gettimeofday(&tm, NULL);
	srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	for (i = 0; i < n; i++){ //Percorre as linhas do Vetor de Ponteiros
		matriz[i] = (double*) malloc(n * sizeof(double)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
		for (j = 0; j < n; j++){ //Percorre o Vetor de Inteiros atual.
			confere+= matriz[i][j] = rand() % 9;
		}
	}
	printf("CONFERE==%.0f\n", confere );
	
	pthread_t thread[PROCESSOS_E_THREADS];
	void *status;
	//inicia o mutex
	pthread_mutex_init(&mutex, NULL);
	
	int t_child[PROCESSOS_E_THREADS];
	
	// int i = 0;

	for(i =0; i < PROCESSOS_E_THREADS; i++) {
		t_child[i] = i;
		pthread_create(&(thread[i]), NULL, ProcessaMatriz, &(t_child[i])); //enviar a matriz
	}
	
	//espera todas as threads acabarem
	for(i = 0; i < PROCESSOS_E_THREADS; i++) {
		pthread_join(thread[i], &status);
	}

	printf("sum==%f\n",sum );
	free(matriz);
	pthread_exit(NULL);

	return 0;
}
