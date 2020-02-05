/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"



int main()
{
	pid_t pid;
	int status;
	while (1) {
		struct cmdline *l;
		int i, j;

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

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);

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
						execvp(cmd[0],cmd);
					} else {
						if(waitpid(pid,&status,0)==-1){
							printf("error");
							exit(-1);
						}
					}
				}
				//printf("\n");
			}
		}
	}
}
