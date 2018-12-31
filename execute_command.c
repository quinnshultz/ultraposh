/*
 * ultraposh - a simple shell
 *
 * Author: Quinn Shultz
 * Contents: User input and command processing functions
 * Filename: ultraposh.c
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "ultraposh.h"

char **segs;

//-----------------------------------------------------------------------------
// NAME
//	execute_command_segment
//
// SYNOPSIS
//	#include "ultraposh.h"
//
//	int execute_command_segment(char segment[], char terminator);
//
// DESCRIPTION
// 	Handles string pointer, redirections, file descriptors, and calls the
//	next appropriate execute function.
//-----------------------------------------------------------------------------
int execute_command_segment(char segment[], char terminator) {
	char *inputFilename = (char *) malloc (sizeof(char)*BUFSIZE);
	char *outputFilename = (char *) malloc (sizeof(char)*BUFSIZE);

	// Tokenize input once more
	char* token = strtok(segment, " ");
	char * tokencopy = (char *) malloc (sizeof(char)*strlen(segment));
	strcpy(tokencopy, token);

	// String pointer for commands and arguments
	segs = (char **) malloc(64);

	int numtokens = 0;
	while (tokencopy != NULL) {
		segs[numtokens] = (char *) malloc(sizeof(char)*strlen(segment));
		segs[numtokens] = tokencopy;
		tokencopy = strtok(NULL, " ");
		numtokens++;
	}
	segs[numtokens] = NULL;

	// End of pipe or single command
	if (terminator == '\0') {

		// Exit command
		if (!strcmp(segs[0], "exit")) {
			exit_ultraposh();

		// Cd command
		} else if (!strcmp(segs[0], "cd")) {
			if (segs[2] != NULL) {
				fprintf(stderr, "ultraposh: cd: too many arguments\n");
				return -1;
			} else if (segs[1] == NULL) {
				change_my_dir("~");
				return 0;
			} else {
				change_my_dir(segs[1]);
				return 0;
			}
			return 0;

		// History command
		} else if (!strcmp(segs[0], "history")) {
			print_history();
			return 0;

		// Help command
		} else if (!strcmp(segs[0], "help")) {
			if (segs[2] != NULL) {
				fprintf(stderr, "ultraposh: help: too many arguments\n");
				return -1;
			} else if (segs[1] == NULL) {
				help();
				return 0;
			} else {
				cmd_help(segs[1]);
				return 0;
			}
			return 0;

		// External command
		} else {
			for (int i = 0; i < numtokens; i++) {

				// Redirect input
				if (strchr(segs[i], '<') != NULL) {
					strcpy(inputFilename, segs[i]);
					if (!strcmp(inputFilename, "<")) {
						strcpy(inputFilename, segs[i+1]);			// Increment pointer to remove first character
						segs[i] = NULL;
						segs[i+1] = NULL;
						numtokens = numtokens - 2;
					} else {
						inputFilename++;
						segs[i] = NULL;
						numtokens--;
					}

					// Open requested file for reading
					inputFD[fdcounter] = open(inputFilename, O_RDONLY);
					outputFD[fdcounter] = dup(0);

				// Redirect output
				} else if (strchr(segs[i], '>') != NULL) {
					strcpy(outputFilename, segs[i]);
					if (!strcmp(outputFilename, ">")) {
						strcpy(outputFilename, segs[i+1]);
						segs[i] = NULL;
						segs[i+1] = NULL;
						numtokens = numtokens - 2;
					} else {
						outputFilename++;
						segs[i] = NULL;
						numtokens--;
					}

					// Open requested file for writing
					outputFD[fdcounter] = open(outputFilename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					inputFD[fdcounter] = dup(0);
				} else {
					inputFD[fdcounter] = dup(0);
					outputFD[fdcounter] = dup(1);
				}
			}

			segarray[fdcounter] = segs;

			for (int j = 0; j <= fdcounter; j++) {
				execute_external_command(inputFD[j], segarray[j], outputFD[j]);
			}

			fdcounter = 0;
			return 0;

		}

	// Pipe
	} else if (terminator == '|') {

		int i = 0;
		while (token != NULL) {

			if (strchr(segs[i], '<') != NULL) {
				strcpy(inputFilename, segs[i]);
				inputFilename++;
				inputFD[fdcounter] = open(inputFilename, O_RDONLY);
				segs[i] = NULL;
				i--;
			}
			i++;
		}

		// Standard input
		if (inputFilename == NULL) {
			inputFD[fdcounter] = dup(0);
		}

		// Close stdout

		segarray[fdcounter] = segs;

		fdcounter++;
		return 0;
	}

	free(tokencopy);

	// User's input was empty or some other error!
	return -1;
}


//-----------------------------------------------------------------------------
// NAME
//	execute_redirected_command
//
// SYNOPSIS
//	#include "ultraposh.h"
//
//	int execute_redirected_command(int inFd, char *args[], int outFd);
//
// DESCRIPTION
//-----------------------------------------------------------------------------
int execute_redirected_command(int inFd, char *args[], int outFd) {

	execute_external_command(inFd, args, outFd);
	return 0;
}


//-----------------------------------------------------------------------------
// NAME
//	execute_external_command
//
// SYNOPSIS
//	#include "ultraposh.h"
//
//	int execute_external_command(int inFd, char *args[], int outFd);
//
// DESCRIPTION
// 	Executes a system call.
//-----------------------------------------------------------------------------
int execute_external_command(int inFd, char *args[], int outFd) {

	// Child process ID
	pid_t c_pid;

	// Create a new process with an identical state to the parent
	c_pid = fork();

	if(c_pid == -1) {
		fprintf(stderr, "ultraposh: error: unable to create child process\n");
		return -1;
	} else if (c_pid == 0) {
		// Child: Return of fork() is zero

		// Redirect stdin and stdout to file descriptors
		dup2(inFd, 0);
		dup2(outFd, 1);

		// Close unused file descriptors
		close(inFd);
		close(outFd);

		for (int i = 0; i <= fdcounter; i++) {
			close(inputFD[fdcounter]);
			close(outputFD[fdcounter]);
		}

		// Execute system call in child process
		execvp(args[0], args);
	} else {
		// Parent: The return of fork() is the process id of the child
		wait(NULL);
	}
	return 0;
}
