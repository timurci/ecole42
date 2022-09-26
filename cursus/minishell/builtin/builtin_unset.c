#include "builtins.h"

int	builtin_unset(t_shell *shell, char **argv)
{
	t_dict	*entry;

	if (!argv)
		return (1);
	argv++;
	while (*argv)
	{
		entry = lst_find_entry(shell->env_list, *argv);
		if (entry)
			lst_destroy_entry(&shell->env_list, *argv);
		else
		{
			entry = lst_find_entry(shell->var_list, *argv);
			if (entry)
				lst_destroy_entry(&shell->var_list, *argv);
		}
		argv++;
	}
	return (0);
}
