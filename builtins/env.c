#include "../minishell.h"

int ft_env(t_env *head)
{
    t_env *t = head;
    while (t)
    {
        if(t->equl_exist == 1)
            printf("%s=%s\n", t->key, t->value);
        t = t->next;
    }
    return 0;
}

// alpha  _
//remove quotation mark 