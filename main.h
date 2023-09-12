#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
typedef struct path
{
	struct path *next;
	char *value;
} path;
int get_command_args(char *line, char ***args);
char *locate_relative_cmd(char *command);
path *_getenv(char *var);
int execute_command(char **argv[],
					char *program_name, char **env);
#endif
