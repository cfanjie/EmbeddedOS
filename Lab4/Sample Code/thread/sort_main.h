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

void *read_keypad_wrapper(void *in)
{
	keypad_input_t *kin = (keypad_input_t *)in;
	read_keypad(&kin->n, &kin->inputs);
}

void call_sorting(unsigned n, int *inputs, 
	char *sort_name, void (*sort)(unsigned size, int *array))
{
	clock_t start = clock();
	
	int *dup_array = duplicate_array(n, inputs);
	sort(n, dup_array);
	printf("\nAfter %s:\n", sort_name);
	print_array(n, dup_array);
	free(dup_array);
	
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%s run in %f second(s)\n", sort_name, seconds);
}

void *call_sorting_wrapper(void *in)
{
	sorting_thread_t *tin = (sorting_thread_t *)in;
	call_sorting(tin->keypad_input->n, tin->keypad_input->inputs, 
		tin->sort_name, tin->fun);
}

