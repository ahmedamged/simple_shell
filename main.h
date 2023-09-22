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
#include <stdarg.h>
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
int get_command_args(char *line, char ***args, char **path,
					 char *program_name);
char *locate_relative_cmd(char *command, char *program_name);
path *_getenv(char *var, char *program_name);
int execute_command(char **path, char **argv[],
					char *program_name, char **env);
bool handle_custom_command(char *line, char ***args, char *program_name);
bool execute_custom_command(char **path, char ***cmd, char *program_name);
void free_args(char **args[]);
void print_env(void);
ssize_t _getline(char **lineptr, ssize_t *len, FILE *file, char *program_name);
void *safe_malloc(size_t size, char *program_name);
void free_path(path **path_temp);
bool is_empty(char *line);
char *safe_cpy(char *dest, char *src, char *program_name);
void free_many(int count, ...);
bool is_path(char *cmd);
#endif
