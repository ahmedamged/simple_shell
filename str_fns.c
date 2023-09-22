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
 * @program_name: destination
 *
 * copys str
 *
 * Return: char *
 */
char *safe_cpy(char *dest, char *src, char *program_name)
{
	int len = strlen(src);

	dest = safe_malloc(sizeof(char) * (len + 1), program_name);
	dest = strcpy(dest, src);
	dest[len] = '\0';
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
	bool is_path = cmd[0] == '/' || strncmp(cmd, "./", 2) == 0 ||
				   strncmp(cmd, "../", 3) == 0;

	return (is_path);
}
