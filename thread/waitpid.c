#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h> /*Variable to store child's pid*/

pid_t childPid;

void childFunc(void) {
  int randtime;
  int exitstatus;

  printf("Chlid: I am child process\n");
  printf("Child: My pid is %d\n", getpid());

  //sleep
  srand(time(NULL));
  randtime = rand() % 5;
  printf("Child: Sleep for %d seconds\n", randtime);
  printf("byebye\n");
  exit(exitstatus);
}

void parentFunc(void) {
  int status;
  pid_t pid;

  printf("Parent: I am the parent process\n");
  printf("Parent: My PID is %d\n", getpid());

  printf("Parent: Wait for child to exit.\n");

  /* Wait for child to exit, and store it's status. */
  do{
    pid = waitpid(childPid, &status, WNOHANG);
    printf("Parent: Waiting child exit.\n");
    sleep(1);//@TODO: Why force to take 1 second sleep?
  }while (pid != childPid);

  if(WIFEXITED(status)) {
    printf("Parent: Child exit at code: %d\n", WEXITSTATUS(status));
  }else {
    printf("Parent: Child process executed but exited failed\n");
  }

  printf("Parent: Bye\n");
  exit(0);
}

int main(void){
	childPid = fork();
	if(childPid >= 0) {
		if(childPid == 0) {
			childFunc();
		}else {
			parentFunc();
		}
	}else {
		perror("fork");
		exit(0);
	}

	childPid = fork();
	return 0;
}
