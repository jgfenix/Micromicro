CC ?=gcc
CFLAGS += -Wall


VETOR=20000

all: clean serial openmp

serial:
	@echo "******************************************************"
	echo 'Compilando SERIAL e executando com tamanho do vetor=$(VETOR)'
	@echo "" ; $(CC) soma_matriz_original.c -o serial $(CFLAGS)
	@echo "" ; ./serial $(VETOR)
	@echo "******************************************************"

openmp:
	@echo "******************************************************"
	echo 'Compilando OPENMP e executando com tamanho do vetor=$(VETOR)'
	@echo "" ; $(CC) soma_matriz_openmp.c -o soma_matriz_openmp -fopenmp $(CFLAGS)
	@echo "" ; OMP_NUM_THREADS=`grep -c ^processor /proc/cpuinfo` ./soma_matriz_openmp $(VETOR)
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