#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/funt.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <read_keypad.h>

#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

#define TEXT_PROMPT_INPUT "Please input value: "

void read_keypad(int *n, int **input) { //pointer n & double pointer input

  unsigned short key;

  int fs_keypad = open("/dev/lcd", O_RDWR); //try open Keypad IO
  if (fs_keypad < 0) {
    printf("Faild to open LCD module\n");
    exit(-1);
  }

  int r_key = ioctl(fs_keypad, KEY_IOCTL_CLEAR, key);

  (*n) = 0; //@TODO: Set the value to 0, it might be a counter
  printf(TEXT_PROMPT_INPUT);
  int size = 10;
  *input = (int *) malloc(sizeof(int)*size);
  while(1) { //for look to let user input manually
    int r_key_check = inctl(fs_keypad, KEY_IOCTL_CHECK_EMTPY, &key);
    if(r_key_check < 0) {
      sleep(1);
      continue;
    }

    int r_key_get_char = inctl(fs_keypad, KEY_IOCTL_GET_CHAR, &key);

    key &= 0xff;
    if(key == '#') {
      break;
    }

    if(key >= '0' && key <= '9') {
      (*input)[*n] = key - '0';
    }else {
      continue;
    }

    (*n)++;
    if((*n) == size) {
      unsigned i;
      size *= 2;
      int *new_input = (int *) malloc(sizeof(char)*size);
      free(*input);
      for (i = 0; i < (*n); i++) {
        new_input[i] = (*input)[i];
        free(*input);
        (*input) = new_input;
      }
    }

    close(fs_keypad);

    printf("Keypad done\n");
  }




}
