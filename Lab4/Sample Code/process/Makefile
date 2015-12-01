all:
	arm-unknown-linux-gnu-gcc -I. -I/home/lab616/microtime/linux/include -c heap_sort.c
	arm-unknown-linux-gnu-gcc -I. -I/home/lab616/microtime/linux/include -c radix_sort.c
	arm-unknown-linux-gnu-gcc -I. -I/home/lab616/microtime/linux/include -c read_keypad.c
	arm-unknown-linux-gnu-gcc -I. -I/home/lab616/microtime/linux/include -c sort_main.c
	arm-unknown-linux-gnu-gcc -I. -I/home/lab616/microtime/linux/include -o sort_process heap_sort.o radix_sort.o read_keypad.o sort_main.o

computer:
	gcc -c heap_sort.c
	gcc -c radix_sort.c
	gcc -c read_keyboard_as_keypad.c
	gcc -c sort_main.c
	gcc -o sort_process heap_sort.o radix_sort.o read_keyboard_as_keypad.o sort_main.o

clean:
	rm -fr *.o sort_process
