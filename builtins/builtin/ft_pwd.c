#include "../headers/built_in.h"
#include "../headers/structs.h"

int	handle_pwd(char **av)
{
	(void)av;
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror ("pwd");
		return (1);
	}
}