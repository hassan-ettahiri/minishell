#include "../minishell.h"

void sigint_handler(int sig)
{
    if(sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}