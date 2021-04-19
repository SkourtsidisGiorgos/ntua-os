#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int p[2];

void test_and_set(){
	int FLAG=1;
	while (FLAG!=0){
	    read(p[0], &FLAG, sizeof(FLAG));
	}
	FLAG=1;
	write(p[1], &FLAG, sizeof(FLAG));
	
}
void write_pipe(){
	write(p[1], &x, sizeof(int));
}

void print_critical(int i, pid_t me){
	test_and_set();
	for(int j=0; j<5; j++){
		printf("Child%d %d executes a critical section\n", i, me);
	}
	write_pipe();
}

void print_non_critical(int i, pid_t me){
	for(int j=0; j<7; j++){
		printf("Child%d %d executes a non critical section\n", i, me);
	}
}

int main(int argc, char **argv) {
    int status;
    pid_t c[3];
	pipe( p );
    c[0] = fork();
    if (c[0] < 0) { printf("fork error"); }
    else if (c[0] == 0) {//c1 code
		print_critical(1, getpid() );
		non_print_critical(1, getpid() );
		non_print_critical(1, getpid() );
		print_critical(1, getpid() );
		non_print_critical(1, getpid() );
		print_critical(1, getpid() );
        exit(0);
    } else {
        c[1] = fork();
        if (c[1] < 0) { printf("fork error"); }
        else if (c[1] == 0) {//c2 code
		    non_print_critical(2, getpid() );
		    print_critical(2, getpid() );
			non_print_critical(2, getpid() );
		    print_critical(2, getpid() );
			non_print_critical(2, getpid() );
		    print_critical(2, getpid() );
            exit(0);
        } else {
            c[2] = fork();
            if (c[2] < 0) { printf("fork error"); }
            else if (c[2] == 0) {//c3 code
		        print_critical(3, getpid() );
				non_print_critical(3, getpid() );
		        print_critical(3, getpid() );
				non_print_critical(3, getpid() );
		        print_critical(3, getpid() );
				non_print_critical(3, getpid() );
                exit(0);
            } else { //F
			    write_pipe();
                wait(&status);
                exit(0);
            }
            wait(&status);
            exit(0);
        }
        wait(&status);
        exit(0);
    }
	return 0;

}
