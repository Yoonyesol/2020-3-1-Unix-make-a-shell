#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char* prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];

void fatal(char* str) {
	perror(str);
	exit(1);
}

int makelist(char* s, const char* delimiters, char** list, int MAX_LIST) {
	int i = 0;
	int numtokens = 0;
	char* snew = NULL;

	if ((s == NULL) || (delimiters == NULL)) return -1;

	snew = s + strspn(s, delimiters);	/* Skip delimiters */
	if ((list[numtokens] = strtok(snew, delimiters)) == NULL)
		return numtokens;

	numtokens = 1;

	while (1) {
		if ((list[numtokens] = strtok(NULL, delimiters)) == NULL)
			break;
		if (numtokens == (MAX_LIST - 1)) return -1;
		numtokens++;
	}
	return numtokens;
}

void cd(char* path) {
	chdir(path);
} //입력했던 파일명이나 디렉토리로 이동

//static void child_handler(int sig) {
//	int status;
//	pid_t pid;
//
//	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
//	}
//}

int main(int argc, char** argv) {
	int i = 0;
	pid_t pid;
	int background = 0;

	//signal(SIGCHLD, (void*)child_handler);

	while (1) {
		fputs(prompt, stdout);
		fgets(cmdline, BUFSIZ, stdin);
		cmdline[strlen(cmdline) - 1] = '\0';

		i = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
		//공백기준으로 cmdline에서 받은 내용을 cmdvector에 저장

		if (strcmp(cmdvector[0], "exit") == 0) {
			fputs("exit the shell.\n", stdout);
			exit(1);
		} //exit

		if (strcmp(cmdvector[0], "cd") == 0) {
			cd(cmdvector[1]);
			continue;
		} //chdir

		if (strcmp(cmdvector[i - 1], "&") == 0) {
			background = 1;
			cmdvector[i - 1] = NULL;
		} //background check

		switch (pid = fork()) {
		case 0:
			/*signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);*/
			execvp(cmdvector[0], cmdvector);
			fatal("main()");
		case -1:
			fatal("main()");
		default:
			if (background == 1) {
				waitpid(-1, NULL, WNOHANG);
			}
			else {
				wait(NULL);
			}
		}
	}
	return 0;
}

