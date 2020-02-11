/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"



int main()
{
	while (1) {
		struct cmdline *l;
		int i, j;
		pid_t pid;
		int status;

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		
		

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			if (strcmp(cmd[0],"quit")==0){
					exit(0);
			} else {
				//printf("seq[%d]: ", i);
				for (j=0; cmd[j]!=0; j++) {
					//printf("%s ", cmd[j]);
					pid=fork();
    				if(pid==0){
						
						if (l->in){
							printf("in: %s\n", l->in);
							int in=open(l->in,O_RDONLY| O_CREAT,0666);
							Dup2(in,0);
							printf("\n");
						}

						if (l->out) {
							printf("out: %s\n", l->out);
							int out=open(l->out,O_WRONLY| O_CREAT,0666);
							Dup2(out,1);
						}

						execvp(cmd[0],cmd);
					} else {
						if(waitpid(pid,&status,0)==-1){
							printf("error");
							exit(-1);
						}
					}
					
				}
			}
		}
	}
}
