#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#define M 3
#define N 4
#define P 5

int A[M][N];
int B[N][P];
int C[M][P];

void *matmult(void *);

main() {
  int i,j,error; pthread_t thr[M];

  /* initialize the matrices ... */
  for( i=0; i<M; i++)
    for (j=0; j<N; j++)
      A[i][j] = i*j;
  for( i=0; i<N; i++)
    for (j=0; j<P; j++)
      B[i][j] = 2*i*j-2;

  for( i=0; i<M; i++) {  // create the worker threads
    if (error = pthread_create(
        &thr[i], 0, matmult, (void *) (intptr_t) i)) {
      fprintf(stderr, "pthread_create: %s", strerror(error));
      exit(1);
    }
  }
  for( i=0; i<M; i++) // wait for workers to finish jobs
    pthread_join(thr[i],0);

  /* print the results ... */
  printf("A\n");
  for( i=0; i<M; i++) {
    for (j=0; j<N; j++) {
      printf("%3d ",A[i][j]);
    }
    printf("\n");
  }

  printf("B\n");
  for( i=0; i<N; i++) {
    for (j=0; j<P; j++) {
      printf("%3d ",B[i][j]);
    }
    printf("\n");
  }

  printf("C\n");
  for( i=0; i<M; i++) {
    for (j=0; j<P; j++) {
      printf("%3d ",C[i][j]);
    }
    printf("\n");
  }
  
} // end main

void *matmult(void *arg) {
  int row = (intptr_t) arg;
  int col;
  int i;
  int t;

  for(col=0; col < P; col++) {
    t=0;
    for( i=0; i<N; i++)
      t += A[row][i] * B[i][col];
    C[row][col] = t;
  }
  return(0);
}
