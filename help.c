/*
 * ultraposh - a simple shell
 *
 * Author: Quinn Shultz
 * Contents: Functions necessary for the help command
 * Filename: help.c
*/

#include <string.h>
#include <stdio.h>
#include "ultraposh.h"

//-----------------------------------------------------------------------------
// NAME
//	help
//
// SYNOPSIS
//	#include "help.h"
//
//	int help();
//
// DESCRIPTION
//	Prints a list of internal commands.
//-----------------------------------------------------------------------------
int help(void) {
	printf("ultraposh, version 1\n");
	printf("These shell commands are defined internally. Type \'help\' to see this list.\n");
	printf("Type \'help name\' to find out more about the function \'name\'.\n");
	printf("Use \'man -k\' or \'info\' to find out more about commands not in this list.\n");
	printf("\n");
	printf("cd [dir]\n");
	printf("exit\n");
	printf("help\n");
	printf("history\n");
	return 0;
}

//-----------------------------------------------------------------------------
// NAME
//	help
//
// SYNOPSIS
//	#include "help.h"
//
//	int cmd_help(char tok[]);
//
// DESCRIPTION
//	Prints help for a specific internal command.
//-----------------------------------------------------------------------------
int cmd_help(char tok[]) {
	if (!strcmp(tok, "cd")) {
		printf("cd: cd [dir]\n");
		printf("    Change the shell working directory.\n\n");
		printf("    Change the current directory to DIR.  The default DIR is the value of the\n");
		printf("    path smash was executed from.\n\n");
		printf("    Arguments:\n");
		printf("\tDIR\tPath to desired working directory\n");
		return 0;
	} else if (!strcmp(tok, "exit")) {
		printf("exit: exit\n");
		printf("    Exit the shell.\n");
		return 0;
	} else if (!strcmp(tok, "help")) {
		printf("help: help [pattern ...]\n");
		printf("    Display information about builtin commands.\n\n");
		printf("    Displays brief summaries of builtin commands. If PATTERN is\n");
		printf("    specified, gives detailed help on all commands matching PATTERN,\n");
		printf("    otherwise the list of help topics is printed.\n\n");
		printf("    Arguments:\n");
		printf("\tPATTERN\tPattern specifiying a help topic\n");
		return 0;
	} else if (!strcmp(tok, "history")) {
		printf("history: history\n");
		printf("    Display the history list.\n\n");
		printf("    Display the history list with line numbers.\n");
		return 0;
	} else {
		fprintf(stderr, "smash: help: no help topics match \'%s\'. Try \'help help\' or \'man -k %s\' or \'info %s\'.\n", tok, tok, tok);
		return -1;
	}
}
