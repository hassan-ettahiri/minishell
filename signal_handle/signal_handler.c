#include "../minishell.h"


void sigint_handler(int sig)
{
	(void)sig;
	if (flag_sig == 1)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		close(130);
		flag_sig = 0;
	}
}