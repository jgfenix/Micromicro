//escreva um programa que calcula a soma de 
// todos os elementos de uma matriz A(n x n). 
// Considere que o processo 0 recebe como entrada da linha de comando
// o valor n, gera os elementos da matriz de modo randomico e distribui
// a computacao da soma pelos processos

// Na saida, o processo 0 deve imprimir o valor da soma de todos os elementos
//28000
#include <stdio.h>
#include <math.h> //random
#include <stdlib.h> //atoi, exit
#include <time.h> //cronometro

#define TIPO double

int main(int argc, char const *argv[])
{
	//verificacao de passagem de parametros	
	if (argv[1] == NULL || atoi(argv[1]) < 1) {
		printf("ERRO, insira o valor de 'n':\n\nEx: mpirun -np [numero de processos] %s [tamanho do vetor] \n", argv[0]);	
		// printf("ERRO, insira o valor de 'n': \n");
		exit(1);
	}

	int n = atoi(argv[1]);

	FILE *in;
	extern FILE *popen();
	char buff[512];

	if(!(in = popen("free -m | awk 'NR==2{ print $4 }'", "r"))){
		exit(1);
	}
	while(fgets(buff, sizeof(buff), in)!=NULL){
		// printf("memoria livre =%d", atoi(buff));
	}
	pclose(in);
	

	printf("\n\nRAM livre:\t\t%d Mb\n", atoi(buff));
	// int n = 1000000;
	if ( ((int)(sizeof(TIPO) * n *n / 1048576)) > atoi(buff))
	{	
		printf("precisa de %f.0 Mb de RAM\n", (TIPO)(sizeof(TIPO) * n *n / 1048576));
		printf("Nao sera possivel continuar o programa, SAINDO...\n\n");
		exit(1);
	}
	printf("\nContinuando...\nAlocando %f.0 Mb RAM ...\n\n", (TIPO)sizeof(TIPO) * n *n / 1048576);

	double sum = 0;
	int i = 0, j=0;
	
	TIPO **matriz ;
	matriz = calloc(n, sizeof(TIPO));
	for (i = 0; i < n; i++)
      matriz[i] = calloc(n, sizeof(TIPO));
	
	// contagem do tempo
	struct timeval tm;
	gettimeofday(&tm, NULL);

	//vai ser usado no random do preenchimento
	// srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
	
	for (i=0; i<n; i++) {
		for (j = 0; j < n; j++)	{
			// matriz[i][j] = random();
			matriz[i][j] = 1.0;  
		}
	}

	//inicio da contagem de tempo
	clock_t begin = clock();
	// #pragma omp parallel for private(i) reduction(+:sum) 
	for (i=0; i<n; i++) {
		for (j = 0; j < n; j++)	{
			sum += matriz[i][j];
			// printf("%f ", matriz[i][j] );
		}
		// printf("\n");
	}


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	free(matriz);
	printf("tempo gasto = %f\n", time_spent );

	printf("sum = %f \n", sum );
	return 0;
}