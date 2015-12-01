#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *show(void *threadid) {
  long tid;
  tid = (long) threadid;
  printf("Hello, I am thread: %ld\n", tid);
  pthread_exit(NULL);
}



int main(void) {
  pthread_t threads[5];
  int rc;
  int t;

  for(t = 0;t < NUM_THREADS;t++) {
    printf("In main: creating thread %d\n", t);
    rc = pthread_create(&threads[t], NULL, show, NULL);
    if(rc) {
      printf("Error: return code from pthread_create is %d\n", rc);
      exit(-1);
    }
  }

  printf("Main: Process exiting\n");
  pthread_exit(NULL);
}
