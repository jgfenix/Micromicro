#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

main(int argc, char* argv[]) 
{
  //n = tamanho do vetor
  //my_rank = id do processo
  //
  double  *x, *y;
  double  dot;
  int     i, n;

  n = atoi(argv[1]);
  x = (double *) calloc(n, sizeof(double));
  y = (double *) calloc(n, sizeof(double));
  struct timeval tm;
  gettimeofday(&tm, NULL);
  srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
  for (i=0; i<n; i++) 
    {x[i] = sin((double)random());  y[i] = sin((double)random());}

  // Envio do valor de n para todos os processos
  
  // Envio dos vetores para todos os processos

  // Calcula o produto escalar local
  clock_t begin = clock();


  dot = 0.0;
  #pragma omp parallel for private(i) reduction(+:dot) 
  // {    
    for (i = 0; i < n; i++)
      dot += sin(x[i] / y[i]);
  // }

  // Coleta resultados
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("%f\n", time_spent );
}  /* main */
