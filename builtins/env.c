#include "../minishell.h"

void print_env(char *s1, char *s2, int flag)
{
    if (flag == 1)
    {
        write(1, s1, ft_strlen(s1));
        write(1, "=", 1);
        write(1, s2, ft_strlen(s2));
        write(1, "\n", 1);
    }
}

int ft_env(t_env *head)
{
    t_env *t = head;
    while (t)
    {
        print_env(t->key, t->value, t->equl_exist);
        t = t->next;
    }
    return 0;
}

// alpha  _
//remove quotation mark 