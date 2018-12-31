/*
 * ultraposh - a simple shell
 *
 * Author: Quinn Shultz
 * Contents: Main function and user input processing functions
 * Filename: ultraposh.c
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ultraposh.h"



//-----------------------------------------------------------------------------
// NAME
//	main, main function, ultraposh
//
// SYNOPSIS
//	#include "ultraposh.h"
//
//	int main();
//
// DESCRIPTION
// 	Gets user input, extracts a command with arguments, and executes it.
//-----------------------------------------------------------------------------
int main(void) {
	// Allocate memory for user-input buffer
	buff = (char *) malloc(sizeof(char)*BUFSIZE);
	init_history();

	// Print first user prompt
	fprintf(stderr,"$ ");

	// Allow user input until end of buffer
	while (fgets(buff,BUFSIZE, stdin) != NULL) {
		buff[strlen(buff) - 1] = '\0';
		add_history(buff);											// Replace newline with NUL

		// Tokenize a command segment
		char* segment = strtok(buff, "|");
		char* segmentcopy = strtok(buff, "|");

		// Count segments
		int numsegments = 0;
		while (segmentcopy != NULL) {
			numsegments++;
			segmentcopy = strtok(NULL, "|");
		}

		inputFD = (int *) malloc(sizeof(int)*numsegments);
		outputFD = (int *) malloc(sizeof(int)*numsegments);
		segarray = (char ***) malloc(64);
		fdcounter = 0;

		// Call execute_command_segment for each segment
		for (int j = 1; j < numsegments; j++) {
			execute_command_segment(segment, '|');
			segment = strtok(NULL, "|");
		}
		if (segment != NULL) {												// If the command is not being piped call function with NUL terminator
			execute_command_segment(segment, '\0');
			segment = strtok(NULL, "\0");
		}

		free(inputFD);
		free(outputFD);
		free(segarray);

		// TODO: Start reapThread
		// Wait reapThread exit

		// Print user prompt
		fprintf(stderr,"$ ");

	}
}


//-----------------------------------------------------------------------------
// NAME
//	exit_ultraposh
//
// SYNOPSIS
//	#include "ultraposh.h"
//
//	void exit_ultraposh();
//
// DESCRIPTION
// 	Frees any remaining data structures and closes the shell.
//-----------------------------------------------------------------------------
void exit_ultraposh(void) {
	free(buff);			// Deallocate memory for buffer
	clear_history();
	exit(0);
}
