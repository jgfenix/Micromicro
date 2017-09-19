/* parallel_dot.c -- compute a dot product of a vector distributed among
 *     the processes.  Uses a block distribution of the vectors.
 *
 * Input: 
 *     n: global order of vectors
 *     x, y:  the vectors
 *
 * Output:
 *     the dot product of x and y.
 *
 * Note:  Arrays containing vectors are statically allocated.  Assumes
 *     n, the global order of the vectors, is divisible by p, the number
 *     of processes.
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"

main(int argc, char* argv[]) 
{
  //n = tamanho do vetor
  //my_rank = id do processo
  //
  double *x, *y, *local_x, *local_y;
  int    n_bar;  /* = n/p */
  double  dot, local_dot;
  int    p, my_rank, i, n, tag=0;
  MPI_Status status;

  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    n = atoi(argv[1]);
    x = (double *) calloc(n, sizeof(double));
    y = (double *) calloc(n, sizeof(double));
    for (i=0; i<n; i++) {x[i] = 1.0;  y[i] = 1.0;}
  }

  // Envio do valor de n para todos os processos
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // printf("ID %d | %d\n", my_rank, n );

  n_bar = n/p;
  // printf("n_bar %d\n", n_bar );
  local_x = (double *) calloc(n_bar, sizeof(double));
  local_y = (double *) calloc(n_bar, sizeof(double));
  
  // Envio dos vetores para todos os processos
  MPI_Scatter(x, n_bar, MPI_DOUBLE, local_x, n_bar, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  MPI_Scatter(y, n_bar, MPI_DOUBLE, local_y, n_bar, MPI_DOUBLE, 0, MPI_COMM_WORLD );

  // for (i= 0; i < n_bar; ++i)
  //  {
  //    printf("proc=%d | i=%d | x[%d]=%f | y[%d]=%f \n", my_rank, i, i, local_x[i], i, local_y[i] );
  //  } 
  // printf("\n");
  // Calcula o produto escalar local

  local_dot = 0.0;
  for (i = 0; i < n_bar; i++)
    local_dot += local_x[i] * local_y[i];

  //printf("myid %d local_dot = %f \n", my_rank,local_dot);

  free(local_x); 
  free(local_y); 
  
  // Coleta resultados
  MPI_Reduce(&local_dot, &dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (my_rank == 0)
  {
    printf("local dot 0 %f\n",dot );
  }


  MPI_Finalize();
}  /* main */