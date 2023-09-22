#include "main.h"
/**
 * is_empty - check if str is empty
 * @line: str to check
 *
 * checks empty strs
 *
 * Return: bool
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
/**
 * safe_cpy - copy str
 * @dest: destination
 * @src: destination
 *
 * copys str
 *
 * Return: char *
 */
char *safe_cpy(char *dest, char *src)
{
	dest = safe_malloc(sizeof(char) * (strlen(src) + 1));
	dest = strcpy(dest, src);
	return (dest);
}
/**
 * is_path - check path
 * @cmd: command
 *
 * checks if command is
 * a "path"ed one
 *
 * Return: bool
 */
bool is_path(char *cmd)
{
	bool is_path = cmd[0] == '/' || strncmp(cmd, "./", 2) == 0
	|| strncmp(cmd, "../", 3) == 0;

	return (is_path);
}
