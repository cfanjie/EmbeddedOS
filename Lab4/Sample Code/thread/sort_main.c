#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <time.h>
#include <pthread.h>
#include "read_keypad.h"
#include "radix_sort.h"
#include "heap_sort.h"
#define NUMBER_OF_SORTING 3

int *duplicate_array(unsigned size, int *array);
void print_array(unsigned size, int *array);
void bubble_sort(unsigned size, int *array);

#include "sort_main.h"

int main(int argc, char **argv)
{
	struct keypad_input my_keypad_input;

	pthread_t thread_keypad, tid_radix, tid_bubble, tid_heap;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	printf("In main() creating thread for Keypad\n");
	int ret = pthread_create(&thread_keypad, &attr, read_keypad_wrapper,
				(void *)&my_keypad_input);
	if(ret)
	{
		printf("Error return code from pthread_create(thread_keypad): %d", ret);
		exit(-1);
	}

	void *status;
	ret = pthread_join(thread_keypad, &status);
	if(ret)
	{
		printf("Error return code from pthread_join(): %d", ret);
		exit(-1);
	}
	printf("Main: join with thread Keypad\n");

	int i;
	sorting_thread_t sorting_threads[NUMBER_OF_SORTING];
	sorting_threads[0] = (sorting_thread_t){ .sort_name = "Radix Sort", .fun = radix_sort };
	sorting_threads[1] = (sorting_thread_t){ .sort_name = "Bubble Sort", .fun = bubble_sort };
	sorting_threads[2] = (sorting_thread_t){ .sort_name = "Heap Sort", .fun = heapSort };
	for(i=0; i<NUMBER_OF_SORTING; i++)
	{
		printf("In main() creating thread for %s\n", sorting_threads[i].sort_name);
		sorting_threads[i].keypad_input = &my_keypad_input;
		ret = pthread_create(&sorting_threads[i].tid, &attr,
			call_sorting_wrapper, (void *)&sorting_threads[i]);
		if(ret)
		{
			printf("Error return code from pthread_create(): %d", ret);
			exit(-1);
		}
	}
	pthread_attr_destroy(&attr);

	//Join threads.
	for(i=0; i<NUMBER_OF_SORTING; i++)
	{
		void *status;
		ret = pthread_join(sorting_threads[i].tid, &status);
		if(ret)
		{
			printf("Error return code from pthread_join(): %d", ret);
			exit(-1);
		}
		printf("Main: join with thread %s\n", sorting_threads[i].sort_name);
	}

	free(my_keypad_input.inputs);
	printf("main() exit normally\n");
	return 0;
}

void bubble_sort(unsigned n, int *data)
{
    int i, j, temp;
    for (i = n - 1; i > 0; i--)
    {
        for (j = 0; j <= i - 1; j++)
        {
            if (data[j] > data[j + 1])
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

int *duplicate_array(unsigned size, int *array)
{
	int *new_array = (int *)malloc(sizeof(int)*size);
	memcpy(new_array, array, sizeof(int)*size);
	return new_array;
}

void print_array(unsigned size, int *array)
{
	unsigned i;
	for(i=0;i<size;i++)
	{
		printf("%d", array[i]);
	}
	printf("\n");
}
