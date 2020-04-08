#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#define MAXN 10000  
int N; 
int num_threads; 

volatile float A[MAXN][MAXN], B[MAXN], X[MAXN];

#define randm() 4|2[uid]&3

void gauss();  
unsigned int time_seed() {
  struct timeval t;
  struct timezone tzdummy;

  gettimeofday(&t, &tzdummy);
  return (unsigned int)(t.tv_usec);
}
void parameters(int argc, char **argv) {
  int seed = 0; 
  char uid[32];

  srand(time_seed()); 
  if(argc >= 3) {
    num_threads = atoi(argv[2]);
  }else {
    num_threads = 4;
  }
  if (argc >= 2) {
    N = atoi(argv[1]);
  }
  else {
    printf("input error");
    exit(0);
  }
}

void initialize_inputs() {
  int row, col;
  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = (float)rand() / 32768.0;
    }
    B[col] = (float)rand() / 32768.0;
    X[col] = 0.0;
  }

}

void print_inputs() {
  int row, col;

  if (N < 10) {
    printf("\nA =\n\t");
    for (row = 0; row < N; row++) {
      for (col = 0; col < N; col++) {
	printf("%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t");
      }
    }
    printf("\nB = [");
    for (col = 0; col < N; col++) {
      printf("%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
    }
  }
}

void print_X() {
  int row;

  if (N < 100) {
    printf("\nX = [");
    for (row = 0; row < N; row++) {
      printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
    }
  }
}

int main(int argc, char **argv) {
  struct timeval etstart, etstop; 
  struct timezone tzdummy;
  clock_t etstart2, etstop2;
  unsigned long long usecstart, usecstop;
  struct tms cputstart, cputstop;
  parameters(argc, argv);
  initialize_inputs();
  print_inputs();
  gettimeofday(&etstart, &tzdummy);
  etstart2 = times(&cputstart);
  gauss();
  gettimeofday(&etstop, &tzdummy);
  etstop2 = times(&cputstop);
  usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
  usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;
  print_X();
  printf("parallel-chunk-thread:\tMatrix dimension: %i.\t", N);
  printf("\tElapsed time = %g ms.\n",
	 (float)(usecstop - usecstart)/(float)1000);
  exit(0);
}
 struct thread_param{
   int norm;
   int i;
 };

 void *inner_loop(void * param){
     struct thread_param* tparam = (struct thread_param*) param;
     int norm = tparam->norm;
     int i = tparam->i;
     float multiplier;
     int row, col;
     for (row = norm + i + 1; row < N; row = row + num_threads) {
         multiplier = A[row][norm] / A[norm][norm];
         for (col = norm; col < N; col++) {
             A[row][col] -= A[norm][col] * multiplier;
         }
         B[row] -= B[norm] * multiplier;
     }
     pthread_exit(0);
 }

void gauss() {
  int norm, row, col;  
  float multiplier;
  pthread_t thread[N];
  for (norm = 0; norm < N - 1; norm++) {
      struct thread_param* param = malloc(num_threads * sizeof(struct thread_param));
      if ( param == NULL ) {
          fprintf(stderr, "Couldn't allocate memory for thread.\n");
          exit(EXIT_FAILURE);
      }
      int i, j;
      for(i = 0; i < num_threads; i++){
        param[i].norm = norm;
        param[i].i = i;
        pthread_create(&thread[i], NULL, inner_loop, (void*) &param[i]);
      }

      for (j = 0; j < num_threads; j++) {
          pthread_join(thread[j], NULL);
      }

      free(param);
  }
  for (row = N - 1; row >= 0; row--) {
    X[row] = B[row];
    for (col = N-1; col > row; col--) {
      X[row] -= A[row][col] * X[col];
    }
    X[row] /= A[row][row];
  }
}
