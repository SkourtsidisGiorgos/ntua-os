#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>


extern int child_number;


void check_arg(int n,int argc);
void check_special(int k);
void handler(int sig);
pid_t create_child(int i);
pid_t * create_children(int n);
int** create_pipes(int arithmos_paidion);
void write_to_pipe(int f,int c);
int read_from_pipe(int fd);
void father_writes_first(int **pipe);
void Ypologismos_Paragontikou(pid_t *c,int n,int k,int **pipe);
void delete_all(pid_t *c,int n,int**pipes);


#endif 
