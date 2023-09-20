#include "main.h"

/**
 * safe_malloc - malloc with protection
 * @size: size of space
 * @program_name: name of program for error
 *
 * safe malloc
 *
 * Return: pointer to new area
 */
void *safe_malloc(size_t size)
{
	void *temp = malloc(size);
	if (temp == NULL)
	{
		perror(program_name);
		exit(1);
	}
	return temp;
}
/**
 * free_path
 */
void free_path(path **path_temp)
{
	path *temp;
	while (*path_temp != NULL)
	{
		free((*path_temp)->value);
		temp = (*path_temp);
		(*path_temp) = (*path_temp)->next;
		free(temp);
	}
}
/**
 * free_args - memory
 * @args: pointer to free
 *
 * frees memory of args
 *
 * Return: void
 */
void free_args(char **args[])
{
	size_t i;

	if (*args != NULL)
	{
		for (i = 0; (*args)[i] != NULL; i++)
		{
			free((*args)[i]);
		}
		free(*args);
	}
}
/**
 * is_empty
 */
bool is_empty(char *line)
{
	while (*line != '\0')
	{
		if (!isspace(*line))
		{
			return (false);
		}
		line++;
	}
	return (true);
}