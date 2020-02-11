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
		if (l->err){
			/* Syntax error, read another command */
						printf("error:\n");
						continue;
		}
		

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			for (j=0; cmd[j]!=0; j++) {
				//printf("%s ", cmd[j]);
				if (strcmp(cmd[0],"quit")==0){
					exit(0);
				}
				pid=fork();
    			if(pid==0){
					//printf("cmd= %s ", l->seq[i][0]);
					if (l->in){
						printf("in: %s\n", l->in);
						int in=open(l->in,O_RDONLY);
						if (in<0){
							Dup2(1,2);
							printf("%s: Permission denied\n",l->in);
							exit(0);
						}
						Dup2(in,0);
					}

					if (l->out) {
						printf("out: %s\n", l->out);
						int out=open(l->out,O_WRONLY| O_CREAT,0666);
						if (out<0){
							Dup2(1,2);
							printf("%s: Permission denied\n",l->out);
							exit(0);
						}
						Dup2(out,1);
					}
					if (execvp(cmd[0],cmd)==-1) { //remplace le l->err
						/* Syntax error, read another command */
						printf("%s: command not found\n", cmd[0]);
					}
					exit(0);
				} else {
					if(waitpid(pid,&status,0)==-1){
						printf("error\n");
						exit(-1);
					}
				}
			}
		}
	}
}
