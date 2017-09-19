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
  if (my_rank == 0)
  {
    for (i = 1; i < p; ++i)
    {
      MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD) ;
    }
  }
  else {
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status) ;
  }

  printf("n= %d | rank= %d\n",n, my_rank );

  n_bar = n/p;
  local_x = (double *) calloc(n_bar, sizeof(double));
  local_y = (double *) calloc(n_bar, sizeof(double));
  
  // Envio dos vetores para todos os processos
  if (my_rank == 0)
  {
    for (i = 1; i < p; ++i)
    {
      MPI_Send( &(x[(i*n_bar)]), n_bar, MPI_DOUBLE, i, 0, MPI_COMM_WORLD) ;
      MPI_Send( &(y[(i*n_bar)]), n_bar, MPI_DOUBLE, i, 0, MPI_COMM_WORLD) ;
    }
    //carrega a primeira parte do vetor no processo 0
    for (i = 0; i < n_bar; ++i)
    {
      local_x[i] = x[i];
      local_y[i] = y[i];
    }
  }
  //todos os processos recebem os valores dos vetores X e Y
  else {
    MPI_Recv(local_x, n_bar, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status) ;
    MPI_Recv(local_y, n_bar, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status) ;
  }  
 
  // Calcula o produto escalar local
  local_dot = 0.0;
  for (i = 0; i < n_bar; ++i)
  {
    local_dot+= local_x[i] * local_y[i];
  }
  printf("%f\n", local_dot);

  if (my_rank != 0)
  {
      MPI_Send( &local_dot, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD) ;
  }
  else 
  {
    dot=local_dot;
    for ( i = 1; i < p; ++i)
    {
  // Coleta resultados
      MPI_Recv(&local_dot, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status) ;
      dot+=local_dot;
    }
    printf("dot total =  %f\n", dot);
  }
  
  // printf("myid %d local_dot = %f \n", my_rank,local_dot);
  free(local_x); 
  free(local_y); 

  MPI_Finalize();
}  /* main */