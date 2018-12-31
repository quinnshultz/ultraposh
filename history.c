/*
 * ultraposh - a simple shell
 *
 * Author: Quinn Shultz
 * Contents: Functions necessary for the history command
 * Filename: history.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ultraposh.h"

struct cmd {
	char * cmd;
};

char *copy;
int linenumber = 0;
struct cmd *cmdptr;
struct cmd histbuff[MAXLINE];

//-----------------------------------------------------------------------------
// NAME
//	init_history
//
// SYNOPSIS
//	#include "history.h"
//
//	int init_history();
//
// DESCRIPTION
//-----------------------------------------------------------------------------
int init_history(void) {
	cmdptr = (struct cmd *) malloc (sizeof(struct cmd));
	return 0;
}


//-----------------------------------------------------------------------------
// NAME
//	add_history
//
// SYNOPSIS
//	#include "history.h"
//
//	int add_history(char buf[]);
//
// DESCRIPTION
//	Add a line from user input to the history buffer.
//-----------------------------------------------------------------------------
int add_history(char buf[]) {
	// Necessary to create a copy of the input so it isn't overwritten
	char *copy = (char *) malloc (BUFSIZE);
	strcpy(copy, buf);

	// Struct cmd pointer to store user commands
	cmdptr->cmd = copy;

	// Add pointer defined above to history buffer
	histbuff[linenumber] = *cmdptr;
	linenumber++;
	return 0;
}


//-----------------------------------------------------------------------------
// NAME
//	clear_history
//
// SYNOPSIS
//	#include "history.h"
//
//	int clear_history();
//
// DESCRIPTION
//	Free data structures and clear out the history buffer.
//-----------------------------------------------------------------------------
int clear_history(void) {
	free(cmdptr);
	for(int i = 0; i < linenumber; i++) {
		free(histbuff[i].cmd);
	}
	return 0;
}


//-----------------------------------------------------------------------------
// NAME
//	print_history
//
// SYNOPSIS
//	#include "history.h"
//
//	int print_history();
//
// DESCRIPTION
//	Prints history buffer contents to stdout.
//-----------------------------------------------------------------------------
int print_history(void) {
	for(int i = 0; i < linenumber; i++) {
		printf(" %d  %s\n",i,histbuff[i].cmd);
	}
	return 0;
}
