#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("eu sou %d de %d\n", rank, size );
	int i;
	if (rank == 0)
	{
		for (i = 0; i < 99; ++i)
		{
			printf("00 %d\n",i);
		}
	}

	if (rank == 1)
	{
		for (i = 100; i < 199; ++i)
		{
			printf("01 %d\n",i);
		}
	}

	// if (rank == 2)
	// {
	// 	for (i = 200; i < 299; ++i)
	// 	{
	// 		printf("02 %d\n",i);
	// 	}
	// }

	MPI_Finalize();
	return 0;
}