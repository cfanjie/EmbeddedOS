#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include "read_keypad.h"

void read_keypad(int *n, int **inputs)
{
	clock_t start = clock();
	
	(*n) = 0;
	unsigned short key;
	printf("Please input from keypad:\n");
	int size=10;
	*inputs = (int *) malloc(sizeof(int)*size);
	while(1)
	{
		key = getchar();

		key &= 0xff;
		if (key == '#')
			break;

		if (key>='0' && key<='9')
			(*inputs)[*n] = key-'0';
		else
			continue;

		printf("%d", (*inputs)[*n]);

		(*n) = (*n) +1;
		if((*n)==size)
		{
			unsigned i;			
			size *= 2;
			int *new_inputs = (int *) malloc(sizeof(char)*size);
			free(*inputs);
			for (i=0; i<(*n); i++)
				new_inputs[i] = (*inputs)[i];
			free(*inputs);
			(*inputs) = new_inputs;
		}
	}
	
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("\nread_keypad run in %f second(s)\n", seconds);
}