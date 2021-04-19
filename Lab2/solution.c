#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

pid_t c[6];
int child_num=-10;
int n=1;

void handler_cont(int sigint){
    return;
}

void print_message(int i){
    child_num=i;
    pause();
    while (1) {
        printf("Child%d %d is executed (%d)\n", child_num, (int) getpid(), n++);
        sleep(1);
    }
}

int main(int argc, char *argv[]) {

    signal(SIGCONT, handler_cont);
    int status = 0;
    int a[argc],count[argc];

    for(int i=1; i<=argc; i++){
      count[i]=0;
    }

    if(argc!=6){
        printf("Error : invalid arguments given\n");
        return 1;
    }

    for(int i=1; i<argc; i++) {
      a[i]=*argv[i]-'0';
      count[a[i]]++;
      if(count[a[i]]>1){
        printf("Error : invalid arguments given\n");
        return 1;
      }
    }

    printf("Parent PID: %d\n", (int) getpid());

    c[1] = fork();
    if (c[1]<0){
      perror("Fork error");
      exit(1);
    }
    else if (c[1] == 0) {
        print_message(1);
    }

    c[3] = fork();
    if (c[3]<0){
      perror("Fork error");
      exit(1);
    }
    else if (c[3] == 0) {
        print_message(3);
    }

    c[4] = fork();
    if (c[4]<0){
      perror("Fork error");
      exit(1);
    }
    else if (c[4] == 0) {
        print_message(4);
    }

    c[2] = fork();
    if (c[2]<0){
      perror("Fork error");
      exit(1);
    }
    else if (c[2] == 0) {
        print_message(2);
    }

    c[5] = fork();
    if (c[5]<0){
      perror("Fork error");
      exit(1);
    }
    else if (c[5] == 0) {
        print_message(5);
    }

    sleep(0);

    for (int child=0; child<4; child++) {
        for (int j = 1; j<=argc-1; j++) {
            kill(c[a[j]], SIGCONT);
            sleep(3);
            if (child < 3){
                kill(c[a[j]], SIGSTOP);
            }
            else{
                kill(c[a[j]],SIGKILL);
            }
        }
    }

    sleep(3);
    i = wait(&status);
    printf("Child died %d\n",i);
    sleep(3);
    i = wait(&status);
    printf("Child died %d\n",i);
    sleep(3);
    i = wait(&status);
    printf("Child died %d\n",i);
    sleep(3);
    i = wait(&status);
    printf("Child died %d\n",i);
    sleep(3);
    i = wait(&status);
    printf("Child died %d\n",i);
    sleep(3);

return 0;
}