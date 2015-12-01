#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "read_keypad.h"
#include "radix_sort.h"
#include "heap_sort.h"

void call_sorting(unsigned n, int *inputs);
void print_array(unsigned size, int *array);
void bubble_sort(unsigned size, int *array);

int main(int argc, char **argv)
{
	int n = -1;
	int *inputs = NULL;
	
	read_keypad(&n, &inputs);
	
	pid_t pid = fork();
	if (pid >= 0)	//fork success
	{
		if (pid == 0)
		{
			call_sorting(n, inputs);
			exit(0);
		}
		else
		{
			if(inputs!=NULL)
				free(inputs);
		}
	}
	else			//fork returns -1 on failure
	{
		perror("fork");
		exit(0);
	}
	
	int status;
	waitpid(pid, &status, WUNTRACED);
	printf("main() exit normally\n");
	return 0;
}

void call_sorting(unsigned n, int *inputs)
{
	printf("\nUnsorted List:\n");
	print_array(n, inputs);
	
	pid_t pid_radix, pid_bubble, pid_heap;
	
	pid_radix = fork();
	if (pid_radix >= 0)	//fork success
	{
		if (pid_radix == 0)	//Child process for "sort the numbers using radix sort"
		{
			clock_t start = clock();
			radix_sort(n, inputs);
			printf("\nAfter radix sort:\n");
			print_array(n, inputs);
			free(inputs);
			clock_t end = clock();
			float seconds = (float)(end - start) / CLOCKS_PER_SEC;
			printf("radix sort run in %f second(s)\n", seconds);
			exit(0);
		}
		else
		{
			pid_bubble = fork();
			if (pid_bubble >= 0)	//fork success
			{
				if (pid_bubble == 0)	//Child process for "sort the numbers using bubble sort"
				{
					clock_t start = clock();
					bubble_sort(n,inputs);
					printf("\nAfter bubble sort:\n");
					print_array(n, inputs);
					free(inputs);
					clock_t end = clock();
					float seconds = (float)(end - start) / CLOCKS_PER_SEC;
					printf("bubble sort run in %f second(s)\n", seconds);
					exit(0);
				}
				else
				{
					pid_heap = fork();
					if (pid_heap >= 0)	//fork success
					{
						if (pid_heap == 0)	//Child process for "sort the numbers using heap sort"
						{
							clock_t start = clock();
							heapSort(inputs,n);
							printf("\nAfter heap sort:\n");
							print_array(n, inputs);
							free(inputs);
							clock_t end = clock();
							float seconds = (float)(end - start) / CLOCKS_PER_SEC;
							printf("heap sort run in %f second(s)\n", seconds);
							exit(0);
						}
						else
						{
							int status;
							waitpid(pid_radix, &status, WUNTRACED);
							if(WIFEXITED(status))	printf("Radix sort exit normally\n");
							waitpid(pid_bubble, &status, WUNTRACED);
							if(WIFEXITED(status))	printf("Bubble sort exit normally\n");
							waitpid(pid_heap, &status, WUNTRACED);
							if(WIFEXITED(status))	printf("Heap sort exit normally\n");
						}
					}
					else
					{
						perror("fork for pid_heap");
						exit(0);
					}
				}
			}
			else			//fork returns -1 on failure
			{
				perror("fork for pid_bubble");
				exit(0);
			}
		}
	}
	else			//fork returns -1 on failure
	{
		perror("fork for pid_radix");
		exit(0);
	}
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

void print_array(unsigned size, int *array)
{
	unsigned i;
	for(i=0;i<size;i++)
	{
		printf("%d", array[i]);
	}
	printf("\n");
}
