#include "main.h"

/**
 * safe_malloc - malloc with protection
 * @size: size of space
 * @program_name: program name
 *
 * safe malloc
 *
 * Return: pointer to new area
 */
void *safe_malloc(size_t size, char *program_name)
{
	void *temp = malloc(size);

	if (temp == NULL)
	{
		p_error();
		exit(1);
	}
	return (temp);
}
/**
 * free_path - free path vars
 * @path_temp: var to free
 *
 * frees path type vars
 *
 * Return: void
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
 * free_many - memory
 * @count: number of pointers
 *
 * frees memory of args
 *
 * Return: void
 */
void free_many(int count, ...)
{
	int i;
	va_list args;

	va_start(args, count);
	for (i = 0; i < count; i++)
		free(va_arg(args, void *));
}
