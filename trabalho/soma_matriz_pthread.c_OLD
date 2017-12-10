#include <stdio.h>
#include <stdlib.h> //atoi, exit, random
#include <sys/time.h> //cronometro
#include <pthread.h>


#ifndef PROCESSOS_E_THREADS
#define PROCESSOS_E_THREADS 2
#endif

struct matriz_e_tamanho {
	double** m; //ponteiro pra matriz
	long n; //tamanho da matriz quadrada
	double sum;
	pthread_mutex_t mutex;
	pthread_t thread[PROCESSOS_E_THREADS];
	pthread_cond_t cond;
};

struct timeval  tv1, tv2;

// TODO(se der tempo): Thread parent so esta gerando e distribuindo a matriz, nao esta processando ela

//controle do numero de threads childs
long tID = 0; //thread 0 é a thread que gera a matriz
int controle_linhas=0;

void *ProcessaMatriz(void *m_n) {
	//struct passada como parametro pra thread executar
	struct matriz_e_tamanho *mat_n = (struct matriz_e_tamanho *)m_n;

	pthread_mutex_lock(&mat_n->mutex);//bloqueia a regiao critica
		long local_tID = tID;
		tID++;
		while(tID< PROCESSOS_E_THREADS) //bloqueia a thread ate que todos tenham um ID
			pthread_cond_wait( &mat_n->cond, &mat_n->mutex );

		pthread_cond_signal(&mat_n->cond);//sinaliza para as threads continuarem pois todas pegaram um ID
	pthread_mutex_unlock(&mat_n->mutex);//desbloqueia a regiao critica

	int i=0, //linhas
		j=0, //colunas
		tamanho_sub_matriz = mat_n->n / PROCESSOS_E_THREADS;
	
	double local_sum=0;
	
	//processamento de dados
	local_sum=0;
	for (i = tamanho_sub_matriz*local_tID; i < ((local_tID+1)*tamanho_sub_matriz) ; ++i) {
		for (j = 0; j < mat_n->n; ++j)
		{
			local_sum += mat_n->m[i][j];
			// printf("thread %ld | m[%d][%d]=%.0f\n",local_tID,i,j, mat_n->m[i][j] );
		}
	}
	//mutex para nao ter mais de uma thread acessando a mesma variavel comum a todas
	pthread_mutex_lock(&mat_n->mutex);//bloqueia a regiao critica
		mat_n->sum+= local_sum;
	pthread_mutex_unlock(&mat_n->mutex);//desbloqueia a regiao critica

	return NULL;
}

void *CriaMatriz(void *tamanhoMatriz) {
	long n ;
	double confere=0; //variavel so pra conferir se as threads estao somando corretamente
	n = (long)tamanhoMatriz;
	int i, j;
 
	double **matriz = (double**)malloc(n * sizeof(double*)); //Aloca um Vetor de Ponteiros
	//vai ser usado no random do preenchimento
	struct timeval tm;
	gettimeofday(&tm, NULL);
	srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	for (i = 0; i < n; i++){ //Percorre as linhas do Vetor de Ponteiros
		matriz[i] = (double*) malloc(n * sizeof(double)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
		for (j = 0; j < n; j++){ //Percorre o Vetor de Inteiros atual.
			confere+= matriz[i][j] = 1.0;//rand() % 9;
		}
	}
	//printf("CONFERE==%.0f\n", confere );
	//struct para passar a matriz e o tamanho dela
	struct matriz_e_tamanho* mat_n = malloc( sizeof( struct matriz_e_tamanho ));
	mat_n->m = matriz;
	mat_n->n = n;
	mat_n->sum = 0;
	
	//inicia o mutex
	pthread_mutex_init(&mat_n->mutex, NULL);
	//inicia a condicao de espera
	pthread_cond_init(&mat_n->cond, NULL);

	//criacao de novas threads
	int sub_threads = PROCESSOS_E_THREADS;

	void *status;
	
	int t_child =0;
	//inicio da contagem de tempo
	gettimeofday(&tv1, NULL);


	for(t_child =0; t_child < sub_threads; t_child++) {
		pthread_create(&(mat_n->thread[t_child]), NULL, ProcessaMatriz, mat_n); //enviar a matriz
	}
	
	//espera todas as threads acabarem
	for(t_child = 0; t_child < sub_threads; t_child++) {
		pthread_join(mat_n->thread[t_child], &status);
	}

	gettimeofday(&tv2, NULL);
	printf ("Total time = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));

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

	pthread_create(&thread0, NULL, CriaMatriz, (void *)n);

	pthread_join(thread0, &status);

	pthread_exit(NULL);
}