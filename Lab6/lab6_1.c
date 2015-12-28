#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define delim " ,:\n\t"
#define MAX_NUM_SCORE 20

char* filename;
int n;

typedef struct DATA{
	int id;
	int score[MAX_NUM_SCORE];
	int mean;
}_data;

void init(_data *d, int len){
	int i, j;
	for(i=0; i<len; i++){
		d[i].id = -1;
		for(j=0; j<MAX_NUM_SCORE; j++){
			d[i].score[j] = -1;
		}
		d[i].mean = -1;
	}
}

void *tfunc(void *t){
	//int data[10][20] = {0};
	_data data[10];
	init(data, 10);
	int tid = (int)t;
	char s[100], *temp;
	int i, j;
	//printf("%d is start!\n", tid);
//-------------------- READ FILE --------------------//
	FILE *fptr;
	if(!(fptr=fopen(filename, "r"))){printf("score.txt not found!\n");exit(1);}
	//printf("%d(FILE OPEN SUCCESS)\n", tid);
	for(i=0; fgets(s, 100, fptr) != NULL; i++){
		//printf("%d - %s", tid, s);
		if((i%n) != (tid%n) || !strcmp(s,"\n")){
		//printf("%d : %d -> X\n", i%n, tid%n);
		continue;
		}
		//printf("%d : %d -> O\n", i%n, tid%n);
		temp = strtok(s, delim);
		data[i/n].id = atoi(temp);
		//printf("%d ", data[i/n].id);
		j = 0;
		//printf("score[%d] ={ ", i/n);
		while((temp=strtok(NULL, delim)) != NULL){
			data[i/n].score[j] = atoi(temp);
			//printf("%d ", data[i/n].score[j]);
			j++;
		}
		//printf("}\n");
	}
//-------------------- READ FILE --------------------//
	//printf("RF done!\n");
//-------------------- CALC MEAN --------------------//
	for(i=0; data[i].id >0; i++){
		//printf("%d: ", data[i].id);
		int sum=0, max=0, min=100;
		for(j=0; data[i].score[j]>=0;j++){
			//printf("%d ", data[i].score[j]);
			sum += data[i].score[j];
			if(data[i].score[j] > max)max = data[i].score[j];
			if(data[i].score[j] < min)min = data[i].score[j];
		}
		//printf(" SUM:%d\n", sum);
		if(j>2)data[i].mean = (sum - max - min)/(j-2);
		else data[i].mean = sum/j;
		//printf("%d: MEAN: %d\n", data[i].id, data[i].mean);
	}
//-------------------- CALC MEAN --------------------//
	for(i=0;data[i].mean>0;i++){
		printf("%d(ID: %d, MEAN: %d)\n", tid, data[i].id, data[i].mean);
	}

}


int main(int argc, char **argv){	//argv[1] = filename, argv[2] = n
	filename = argv[1];
	n = atoi(argv[2]);
	int i = 0;
	printf("GO!\n");
//-------------------- CREAT THREAD --------------------//

	pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t)*n);
	int rc=0;
	void *status;

	for(i=0; i<n; i++){
		rc=pthread_create(thread+i, NULL, tfunc, (void *)i);
		if(rc){printf("ERROR: CREATE(%d)\n", rc); exit(-1);}
	}
	printf("CREATE SUCCESS!\n");
	for(i=0; i<n; i++){
		rc=pthread_join(thread[i], &status);
		if(rc){printf("ERROR:JOIN(%d)\n", rc); exit(-1);}
	}
	printf("JOIN SUCCESS!\n");
//-------------------- CREAT THREAD --------------------//

	free(thread);
	return 0;
}
