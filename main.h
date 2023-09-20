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
#include <fcntl.h>
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
int get_command_args(char *line, char ***args, char **path);
char *locate_relative_cmd(char *command);
path *_getenv(char *var);
int execute_command(char **path, char **argv[],
					char *program_name, char **env);
bool handle_custom_command(char *line, char ***args);
bool execute_custom_command(char ***cmd);
void free_args(char **args[]);
void print_env(void);
ssize_t _getline(char **lineptr, size_t *len, FILE *file);
#endif
