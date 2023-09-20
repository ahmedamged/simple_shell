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