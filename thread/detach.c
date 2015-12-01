#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *threadfunc(void *parm) {
  printf("Inside secondary thread\n");
  sleep(3);
  printf("Exit Secondary thread\n");
  pthread_exit(NULL);
}

int main(void) {
  pthread_t thread;
  int rc = 0;

  printf("Create a thread using attributes that allow detach\n");
  rc = pthread_create(&thread, NULL, threadfunc, NULL);
  if(rc) {
    printf("Error: return code from pthread_create() is %d\n", rc);
    exit(-1);
  }

  printf("Detach the thread after it terminates\n");
  rc = pthread_detach(thread);
  if(rc) {
    printf("Error: return code from pthread_detach()\n");
    exit(-1);
  }

  printf("Detach the thread again\n");
  rc = pthread_detach(thread);
  /*No thread could be found by the given thread ID*/

  if(rc != EINVAL) {
    printf("Got an unexpected result rc = %d\n", rc);
    exit(-1);
    }

    printf("Second detac fails as expected\n");

    sleep(6);
    printf("Main() complete\n");
    return 0;
}
