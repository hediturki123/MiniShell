/*
 * Copyright (C) 2002, Simon Nieuviarts
 */
/********************************************************************
 * Régler le problème d'affichage des erreurs (command not found) et cat d'un mauvais fichier s'affiche 2 fois
 * 
 * 
 ******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "readcmd.h"
#include "csapp.h"



int main()
{
	while (1) {
		struct cmdline *l;
		int i;
		pid_t pid;
		int status;
		int tube[2];
		int tube2[2];

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}
		
		

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			if (strcmp(cmd[0],"quit")==0){
				exit(0);
			}
			if(i!=0 && l->seq[i-1]!=0){ // création du deuxième tube si plus de un tube
				if (pipe(tube2) == -1){
						perror("pipe2 error");
					}	
			}
			if(l->seq[i]!=0){ // création du tube si pipe
				if (pipe(tube) == -1){
						perror("pipe error");
					}
			}
			pid=fork();
    		if(pid==0){
				//printf("cmd= %s ", l->seq[i][0]);
				if (l->in){
					//printf("in: %s\n", l->in);
					int in=open(l->in,O_RDONLY);
					if (in<0){
						perror(l->in); //affiche le message d'erreur
						exit(0);
					}
					Dup2(in,0);
				}
				if (l->out) {
					//printf("out: %s\n", l->out);
					int out=open(l->out,O_WRONLY| O_CREAT,0666);
					if (out<0){
						perror(l->out);
						exit(0);
					}
					Dup2(out,1);
				}
				
				if(l->seq[i+1]!=0){
					close(tube[0]); //ferme sortie tube
					Dup2(tube[1],1);
				}
				if (execvp(cmd[0],cmd)==-1) { //remplace le l->err
					/* Syntax error, read another command */
					perror(cmd[0]);
				}
				exit(0);
			} else { // père
				if(waitpid(pid,&status,0)==-1){
					printf("error\n");
					exit(-1);
				}
				if(l->seq[i+1]!=0){
					close(tube[1]);
					Dup2(tube[0],0);
				}
			}
		}
	}
}
