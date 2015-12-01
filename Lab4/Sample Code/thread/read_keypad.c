#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#define LCD_DEVICE_NAME "/dev/lcd"
#include "read_keypad.h"

void read_keypad(int *n, int **inputs)
{
	int fd_keypad, ret;
	unsigned short key;
	
	clock_t start = clock();
	if ((fd_keypad=open(LCD_DEVICE_NAME, O_RDWR))<0)
	{
		printf("Open LCD device[%s] fail\n", LCD_DEVICE_NAME);
		exit(-1);
	}
	ret = ioctl(fd_keypad, KEY_IOCTL_CLEAR, key);
	
	(*n) = 0;
	printf("Please input from keypad:\n");
	int size=10;
	*inputs = (int *) malloc(sizeof(int)*size);
	while(1)
	{
		ret = ioctl(fd_keypad, KEY_IOCTL_CHECK_EMTPY, &key);
		if (ret < 0)
		{
			sleep(1);
			continue;
		}

		ret = ioctl(fd_keypad, KEY_IOCTL_GET_CHAR, &key);

		key &= 0xff;
		if (key == '#')
			break;

		if (key>='0' && key<='9')
			(*inputs)[*n] = key-'0';
		else
			continue;

		printf("%d", (*inputs)[*n]);

		(*n)++;
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
	
	close(fd_keypad);
	
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("\nread_keypad run in %f second(s)\n", seconds);
}

