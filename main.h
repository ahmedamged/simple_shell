#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
/**
 * struct path - Typedef for command args
 * @next: pointer for next entity
 * @value: string
 *
 * Description: struct for passing arguments
 */
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
bool handle_custom_command(char *line, char ***args);
void execute_custom_command(char ***cmd);
void free_args(char **args[]);
#endif
