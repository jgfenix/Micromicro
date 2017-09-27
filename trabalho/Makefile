CC ?=gcc
CFLAGS += -Wall
PROCESSOS_E_THREADS := $(shell grep -c ^processor /proc/cpuinfo)

MATRIZ=10

all: clean serial openmp

serial:
	@echo "******************************************************"
	echo 'Compilando SERIAL e executando com tamanho da matriz=$(MATRIZ) Usando $(PROCESSOS_E_THREADS) processadores'
	@echo "" ; $(CC) soma_matriz_original.c -o serial.exe $(CFLAGS)
	@echo "" ; ./serial.exe $(MATRIZ)
	@echo "******************************************************"

openmp:
	@echo "******************************************************"
	echo 'Compilando OPENMP e executando com tamanho da matriz=$(MATRIZ) Usando $(PROCESSOS_E_THREADS) processadores'
	@echo "" ; $(CC) soma_matriz_openmp.c -o soma_matriz_openmp.exe -fopenmp $(CFLAGS)
	@echo "" ; OMP_NUM_THREADS=$(PROCESSOS_E_THREADS) ./soma_matriz_openmp.exe $(MATRIZ)
	@echo "******************************************************"

mpi:
	@echo "******************************************************"
	@echo 'Compilando MPI e executando com tamanho da matriz=$(MATRIZ) Usando $(PROCESSOS_E_THREADS) processadores'
	@echo "" ; mpicc soma_matriz_mpi.c -o soma_matriz_mpi.exe $(CFLAGS)
	@echo "" ; mpirun -np $(PROCESSOS_E_THREADS) soma_matriz_mpi.exe $(MATRIZ)
	@echo "******************************************************"

ptread:
	@echo "" ; echo 'Compilando PTHREAD e executando com tamanho da matriz=$(MATRIZ)'

clean:
	rm -f a.out *.exe

memory_clean:
	free -m
	sudo echo 3 > /proc/sys/vm/drop_caches
	free -m

.PHONY: clean