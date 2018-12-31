/*
 * ultraposh - a simple shell
 *
 * Author: Quinn Shultz
 * Contents: Function necessary to change the user's PATH
 * Filename: change_my_dir.c
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "ultraposh.h"

//-----------------------------------------------------------------------------
// NAME
//	execute_command_segment
//
// SYNOPSIS
//	#include "change_my_dir.h"
//
//	int change_my_dir(char dir[]);
//
// DESCRIPTION
// 	Changes to the requested directory.
//-----------------------------------------------------------------------------
int change_my_dir(char dir[]) {

        if (chdir(dir) == 0) {				// System call to change directory
        	if (getcwd(cwd, BUFSIZE) != NULL) {
                	printf("%s\n", cwd);
        	}
		return 0;
	} else {
		fprintf(stderr, "ultraposh: cd: unable to change to: %s\n", dir);
		return -1;
	}
}

