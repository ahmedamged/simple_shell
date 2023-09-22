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
#include <ctype.h>
#include <signal.h>
#include <stdint.h>
#define EXEC_ERROR -1
#define NOT_FOUND 404
#define IS_PART_OF_PIPE 0
#define MAX_LINE_LENGTH 1024
#define IS_NEW_LINE 5
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
extern char *program_name;
int get_command_args(char *line, char ***args, char **pathz);
char *locate_relative_cmd(char *command);
path *_getenv(char *var);
int execute_command(char **path, char **argv[],
					char *program_name, char **env);
bool handle_custom_command(char *line, char ***args);
bool execute_custom_command(char **path, char ***cmd, char *program_name);
void free_args(char **args[]);
void print_env(void);
ssize_t _getline(char **lineptr, ssize_t *len, FILE *file);
void *safe_malloc(size_t size);
void free_path(path **path_temp);
bool is_empty(char *line);
#endif
