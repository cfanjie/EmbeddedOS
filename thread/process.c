#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h> /*Primitive system data types*/
#include <sys/wait.h> /*Wait for process termination */
#include <unistd.h> /*Sysbolic constants */

pid_t childPid; //Variable to store the child's pid

void childFunc(void) {
	int retval; /*user provided return code */
	printf("Child: I am the child process\n");
	printf("Child: My pid is %d\n", getpid());
	printf("Child: My parent PID is %d\n", getppid());
	printf("Child: Sleeping for 1 second\n");
	sleep(1);
	scanf("%d", &retval);
	exit(retval); //child exit with given return code
}

void parentFunc(void) {
	int status; /*child exit code */
	printf("Parent: I am parent process\n");
	printf("Parent: My PID is %d\n", getpid());
	printf("Parent: My child's PID is %d\n", childPid);
	printf("Wait for child to exit\n");
	wait(&status); //@TODO: why "&"
	printf("Parent: Child's exit code is %d\n", WEXITSTATUS(status)); //@TODO: Why?
	printf("Parent: Bye\n");
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
