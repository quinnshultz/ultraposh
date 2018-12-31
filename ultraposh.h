/*
 * ultraposh - a simple shell
 *
 * Author: Quinn Shultz
 * Contents: Macro definitions and function prototypes used by ultraposh
 * Filename: ultraposh.h
*/

#ifndef ULTRAPOSH
#define ULTRAPOSH

/* Macro definitions */
#define BUFSIZE 256
#define MAXLINE 4096

/* Global variables */
int fdcounter;
int *inputFD;
int *outputFD;
char *buff;
char ***segarray;
char cwd[BUFSIZE];

/* Function prototypes */
int add_history(char hist[]);
int change_my_dir(char dir[]);
int clear_history(void);
int cmd_help(char tok[]);
int execute_command_segment(char segment[], char terminator);
int execute_redirected_command(int inFd, char *args[], int outFd);
int execute_external_command(int inFd, char *args[], int outFd);
void exit_ultraposh(void);
int help(void);
int init_history(void);
int print_history(void);

#endif
