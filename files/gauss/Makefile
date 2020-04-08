CC:=gcc

program: serial.c parallel-pthreads.c parallel-chunk-threads.c parallel-openmp.c

	 $(CC) serial.c -o serial.o

	 $(CC) parallel-pthreads.c -pthread -o parallel-pthreads.o

	 $(CC) parallel-chunk-threads.c -pthread -o parallel-chunk-thread.o

	 $(CC) parallel-openmp.c -fopenmp -o parallel-openmp.o

clean:
	rm -rf *.out
