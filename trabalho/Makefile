CC ?=gcc
CFLAGS += -Wall
PROCESSOS_E_THREADS := $(shell grep -c ^processor /proc/cpuinfo)

VETOR=20000

all: clean serial openmp

serial:
	@echo "******************************************************"
	echo 'Compilando SERIAL e executando com tamanho do vetor=$(VETOR) Usando $(PROCESSOS_E_THREADS) processadores'
	@echo "" ; $(CC) soma_matriz_original.c -o serial.exe $(CFLAGS)
	@echo "" ; ./serial.exe $(VETOR)
	@echo "******************************************************"

openmp:
	@echo "******************************************************"
	echo 'Compilando OPENMP e executando com tamanho do vetor=$(VETOR) Usando $(PROCESSOS_E_THREADS) processadores'
	@echo "" ; $(CC) soma_matriz_openmp.c -o soma_matriz_openmp.exe -fopenmp $(CFLAGS)
	@echo "" ; OMP_NUM_THREADS=$(PROCESSOS_E_THREADS) ./soma_matriz_openmp.exe $(VETOR)
	@echo "******************************************************"

mpi:
	@echo "******************************************************"
	echo 'Compilando MPI e executando com tamanho do vetor=$(VETOR) Usando $(PROCESSOS_E_THREADS) processadores'
	@echo "" ; mpicc soma_matriz_mpi.c -o soma_matriz_mpi.exe $(CFLAGS)
	@echo "" ; mpirun -np $(PROCESSOS_E_THREADS) soma_matriz_mpi.exe $(VETOR)
	@echo "******************************************************"

ptread:
	@echo "" ; echo 'Compilando PTHREAD e executando com tamanho do vetor=$(VETOR)'

clean:
	rm -f serial original a.out soma_matriz_openmp soma_matriz_mpi soma_matriz_pthread > /dev/null

memory_clean:
	free -m
	sudo echo 3 > /proc/sys/vm/drop_caches
	free -m

.PHONY: clean