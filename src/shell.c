/*
 Projet SR : TURKI SANEKLI Hedi & NOEL-LARDIN Thomas
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

#define ENTREE 1
#define SORTIE 0

void handler(int sig){
	while(waitpid(-1, NULL, WNOHANG|WUNTRACED) > 0){}
}


int main()
{
	signal(SIGCHLD,handler);
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
			if (pipe(tube2) == -1){
					perror("pipe2 error\n");
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
				
				if(i==0 && l->seq[i+1]==0){
					close(tube2[SORTIE]);
					close(tube2[ENTREE]);
				}

				if(i==0 && l->seq[i+1]!=0){
					close(tube2[SORTIE]); //ferme sortie tube
					Dup2(tube2[ENTREE],1); // entrée tube dans sortie standard
				}

				if(i!=0 && l->seq[i+1]==0){ // s'il y a une commande précédente
					close(tube[ENTREE]);
					Dup2(tube[SORTIE],0); // sortie tube dans entrée standard
				}
				
				if(i!=0 && l->seq[i+1]!=0){ 
					close(tube[ENTREE]);
					Dup2(tube[SORTIE],0);
					close(tube2[SORTIE]);
					Dup2(tube2[ENTREE],1);
				}

				if (execvp(cmd[0],cmd)==-1) { //remplace le l->err et execute execvp
					/* Syntax error, read another command */
					perror(cmd[0]);
				}
				exit(0);
			} else { // père
				
				if(tube[0]!=-1) {
					close(tube[SORTIE]);
				}
				if(tube[1]!=-1) {
					close(tube[ENTREE]);
				}

				tube[SORTIE]=tube2[SORTIE];
				tube[ENTREE]=tube2[ENTREE];

				if (l->ampersand == 0){
					if(waitpid(pid,&status,0)==-1){
						printf("error\n");
						exit(-1);
					}
				}
			}
		}
	}
}
