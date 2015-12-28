#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#ifdef PXA
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#endif

#define delim " ,:\n\t"
#define MAX_NUM_SCORE 20
#define ON 1
#define OFF 0

#ifdef USE_SEM
#define SEM_MODE 0666
#define SEM_KEY 0360043l

int sem;

int P(int s){
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = 0;

	if(semop(s, &sop, 1) < 0){
		fprintf(stderr, "P(): semop failed: %s\n", strerror(errno));
		return -1;
	}
	else {
		return 0;
	}
}

int V(int s){
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op = 1;
	sop.sem_flg = 0;

	if(semop(s, &sop, 1) < 0){
		fprintf(stderr, "V(): semop failed: %s\n", strerror(errno));
		return -1;
	}
	else {
		return 0;
	}
}

#endif

#ifdef PXA
int fd;

unsigned int numtable[]={63,6,91,79,102,109,125,7,127,111, 0};
void show7s(int fd, int a){
	ioctl(fd, _7SEG_IOCTL_ON, NULL);
	_7seg_info_t data;
	data.Mode = _7SEG_MODE_PATTERN;
	data.Which = _7SEG_D5_INDEX | _7SEG_D6_INDEX | _7SEG_D7_INDEX | _7SEG_D8_INDEX;
	data.Value = 0x0;
	int i=0,n[4];
	int temp=a;
	for(i=0;i<4;i++){
		n[3-i]=temp%10;
		temp/=10;
	}
	for(i=0; i<4;i++){
		//int temp=a/10;
		data.Value = (data.Value<<8) | numtable[n[i]];
		a/=10;
	}
	ioctl(fd, _7SEG_IOCTL_SET, &data);
}
void show7sc(int fd, int *a){
	ioctl(fd, _7SEG_IOCTL_ON, NULL);
	_7seg_info_t data;
	data.Mode = _7SEG_MODE_PATTERN;
	data.Which = _7SEG_D5_INDEX | _7SEG_D6_INDEX | _7SEG_D7_INDEX | _7SEG_D8_INDEX;
	data.Value = 0x0;
	int i=0,n[4];
/*
	int temp=a;
	for(i=0;i<4;i++){
		n[3-i]=temp%10;
		temp/=10;
	}
*/
	for(i=0; i<4;i++){
		//int temp=a/10;
		data.Value = (data.Value<<8) | numtable[a[i]];
		//a/=10;
	}
	ioctl(fd, _7SEG_IOCTL_SET, &data);
}
#define NUM_ID 7
#define NUM_SCRBIT 3+NUM_ID+4+3

void scrolling(int fd, int id, int mean){
	int bit[NUM_SCRBIT];
	bit[0]=bit[1]=bit[2]=bit[10]=bit[NUM_SCRBIT-3]=bit[NUM_SCRBIT-2]=bit[NUM_SCRBIT-1]=10;
	int i, j, mask = 1000000, temp;
	for(i=3;mask>0;i++, mask/=10){
	temp = id%(mask*10);
	bit[i] = temp /mask;
	}
	mask = 100;
	for(i=11;mask>0;i++, mask/=10){
	temp = mean%(mask*10);
	bit[i] = temp /mask;
	}
	for(i=0;i<17;i++){
		//printf("%d", bit[i]);
	}
	//printf("\nshow:\n");
	for(i=0;i<17-3;i++){
		/*
		int OUTPUT = 0;
		for(j=0;j<4;j++){
			OUTPUT=OUTPUT*10+bit[i+j];
		}
		*/
		//printf("%d\n", OUTPUT);
		show7sc(fd, bit+i);
		unsigned long k=0;
		for(k=0;k<50000000L;k++);
	}
	//printf("\n");
}

void setLED(unsigned short LED, int onoff){
	unsigned short data=LED;
	if(onoff)
		ioctl(fd, LED_IOCTL_BIT_SET, &data);
	else
		ioctl(fd, LED_IOCTL_BIT_CLEAR, &data);
}
#endif

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

void pfunc(){
	//int data[10][20] = {0};
	_data data[10];
	init(data, 10);
	int pid = getpid();
	char s[100], *temp;
	int i, j;

//-------------------- READ FILE --------------------//
	FILE *fptr;
	if(!(fptr=fopen(filename, "r"))){printf("score.txt not found!\n");exit(1);}
	//printf("%d(FILE OPEN SUCCESS)\n", tid);
	for(i=0; fgets(s, 100, fptr) != NULL; i++){
		//printf("%d - %s", tid, s);
		if((i%n) != (pid%n) || !strcmp(s,"\n")){
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
		printf("%d (%d)(ID: %d, MEAN: %d)\n", pid, pid%n, data[i].id, data[i].mean);
#ifdef USE_SEM
		P(sem);
#endif
#ifdef PXA
		setLED((unsigned short)(pid%n), ON);
		scrolling(fd, data[i].id, data[i].mean);
		setLED((unsigned short)(pid%n), OFF);
#endif
#ifdef USE_SEM
		V(sem);
#endif
	}
}


int main(int argc, char **argv){	//argv[1] = filename, argv[2] = n
	filename = argv[1];
	n = atoi(argv[2]);
	int i = 0;
	printf("GO!\n");

#ifdef PXA
	if((fd = open("/dev/lcd", O_RDWR))<0)return(-1);
	unsigned short ini = LED_ALL_OFF;
	//setLED(LED_ALL_OFF, ON);
	ioctl(fd, LED_IOCTL_SET, &ini);
#endif
//--------------------Create Semaphore--------------------//
#ifdef USE_SEM
	sem = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | SEM_MODE);
	if(sem < 0){
		fprintf(stderr, "Creation of semaphore %ld failed: %s\n", SEM_KEY, strerror(errno));
		exit(-1);
	}

	if(semctl(sem, 0, SETVAL, 1) < 0){
		fprintf(stderr, "Unable to initialize semaphore: %s\n", strerror(errno));
	exit(0);
	}

	printf("Semaphore %ld has been created & initialized to 1\n", SEM_KEY);
#endif
//--------------------Create Semaphore--------------------//
//-------------------- CREAT THREAD --------------------//
	int childpid;
	int *pid_table = (int*)malloc(sizeof(int)*n);
	int status;
	for(i=0; i<n; i++){
		if((childpid = fork()) > 0){
			pid_table[i] = childpid;
		}
		else if (childpid == 0){
			pfunc();
			exit(0);
		}
		else{
			perror("fork");
			exit(-1);
		}
	}
	for(i=0; i<n; i++){
		waitpid(pid_table[i], &status, 0);
	}
//-------------------- CREAT THREAD --------------------//
//--------------------Delete Semaphore--------------------//
#ifdef USE_SEM
	if(semctl(sem, 0, IPC_RMID, 0) < 0){
		fprintf(stderr, "%s unable to remove semaphore %ld\n", argv[0], SEM_KEY);
		exit(1);
	}

	printf("Semaphore %ld has been remove.\n", SEM_KEY);
#endif
//--------------------Delete Semaphore--------------------//

	return 0;
}
