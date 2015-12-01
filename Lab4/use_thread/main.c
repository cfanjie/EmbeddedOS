#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "read_keypad.h"
#include "main_sort.h"


typedef struct keypad_input
{
	int n;
	int *inputs;
} keypad_input_t;

typedef struct sorting_thread
{
	pthread_t tid;
	char *sort_name;
	void (*fun)(unsigned, int *);
	keypad_input_t *keypad_input;
} sorting_thread_t;


int main(int argc, char **argv) {
	struct keypad_input st_keypad_input;
  pthread thread_readkey, thread_radix, thread_bubble, thread_heap;

  //Set the universal thread attribute with "JOINABLE"
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //def: pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) -> int;
  int r_key = pthread_create(&thread_readkey, &attr, read_keypad_wrapper, (void *)&st_keypad_input);
  if(r_key) {
    printf("Error while creating key pad thread");
    exit(-1);
  }

  void *join_status;
  int r_key_join = pthread_join(thread_readkey, &join_status);
  if(r_key_join) {
    printf("Error whlie joining keypad reading thread\n");
    exit(-1);
  }

	printf("Keypad thread is created and joined\n");



}

void *read_keypad_wrapper(void *input) {
	keypad_input_t *_input = (keypad_input_t *)input;
	read_keypad(&_input->n, &_input->inputs);
}
