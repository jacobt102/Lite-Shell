#define MAXARGS   128
#define LARGEPATH 256
#include "csapp.h"
/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
int numVars;
sigjmp_buf buf;
int return_pid();
int return_ppid();
char* cd_built();

void sigint_handler(int sig){
	Sio_puts("\n");
	siglongjmp(buf,1);

	//Do nothing when SIGINT is caught

}
int main(int argc, char** argv) 
{
	numVars = argc;
	char cmdline[MAXLINE]; /* Command line */
	if(signal(SIGINT,sigint_handler)==SIG_ERR){
		unix_error("signal error");

	}

	while (1) {
		/* Read */
		sigsetjmp(buf,1);
		if(argc == 3&&strcmp(argv[1],"-p")==0){
			printf("%s> ",argv[2]);

		}
		else{
			printf("sh257> ");


		}
		Fgets(cmdline, MAXLINE, stdin); 
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline);
	} 
}
/* $end shellmain */

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
	char *argv[MAXARGS]; /* Argument list execve() */
	char buf[MAXLINE];   /* Holds modified command line */
	int bg;              /* Should the job run in bg or fg? */
	pid_t pid;           /* Process id */

	strcpy(buf, cmdline);
	bg = parseline(buf, argv); 
	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */

	if (!builtin_command(argv)) { 
		if ((pid = Fork()) == 0) {   /* Child runs user job */
			if (execvp(argv[0], argv) < 0) {
				printf("Execution failed (in fork)\n");
				printf("%s: Command not found.\n", argv[0]);
				exit(1);
			}



		}

		/* Parent waits for foreground job to terminate */
		if (!bg) {
			int status;
			if (waitpid(pid, &status, 0) < 0)
				unix_error("waitfg: waitpid error");
			if(WIFEXITED(status)){
				printf("Process exited with status code %d\n",WEXITSTATUS(status));
			}

		}
		else{
			printf("%d %s", pid, cmdline);
		}
	}

	return;
}

//Built-in Command Definitions

int return_pid(){
	return getpid();

}
int return_ppid(){
	return getppid();
}

char* cd_built(){
	char* holder=getcwd(holder,256);
	return holder;
}
void changedir(char* newpath){
	if(chdir(newpath)!=0){//chdir didn't work
		printf("Path doesn't exist\n");

	}
	return;
}

void exit_built(){
	raise(SIGTERM);

}
void help_in(){
	printf("***************************************\n");
	printf("A custom Shell for CMSC 257\n");
	printf("- Jacob Turner\n");
	printf("Usage:\n- After using make, ./sh257 -p <prompt> will allow you to change the prompt\n");
	printf("***************************************\nBUILTIN COMMANDS:\n");
	printf("exit: Exits the shell\npid: Prints the process ID\nppid: Prints the parent process ID\nhelp: Displays this menu\ncd: Prints current working directory while \"cd <path>\"  changets the current working directory\n");
	printf("SYSTEM COMMANDS:\n- Use man pages for system commands\n");
	fflush(stdout);
	return;
}


/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
	if (!strcmp(argv[0], "exit")){ /* quit command */
		exit_built(); 
	}
	if (!strcmp(argv[0], "&")){    /* Ignore singleton & */
		return 1;
}
	if(!strcmp(argv[0], "pid")){
		printf("%d\n", return_pid());
		return 1;
	}
	if(!strcmp(argv[0], "ppid")){
		printf("%d\n", return_ppid());
		return 1;

	}
	if(!strcmp(argv[0], "help")){

		help_in();
		return 1;
	}
	if(!strcmp(argv[0], "cd")){
		if(argv[1]!=NULL){//Make sure path is given
			changedir(argv[1]);
			return 1;
		}
		printf("Current working directory: %s\n", cd_built());
		return 1;
	}

	return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
	char *delim;         /* Points to first space delimiter */
	int argc;            /* Number of args */
	int bg;              /* Background job? */

	buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
	while (*buf && (*buf == ' ')) /* Ignore leading spaces */
		buf++;

	/* Build the argv list */
	argc = 0;
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* Ignore spaces */
			buf++;
	}
	argv[argc] = NULL;

	if (argc == 0)  /* Ignore blank line */
		return 1;

	/* Should the job run in the background? */
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
	numVars = argc;
}
/* $end parseline */


