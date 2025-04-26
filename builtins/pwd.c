#include "../minishell.h"

char *get_cwd_in_env(t_env *e)
{
    t_env *tmp;

    tmp = e;
    while(tmp){
        if(strcmp(tmp->key, "PWD") == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

int pwd(t_env *e)
{
    char *c = getcwd(NULL, 0);
    if(c != NULL){
        write(1, c, ft_strlen(c));
        write(1, "\n", 1);
    }
    else
    {
        free(c);
        c = get_cwd_in_env(e);
        write(1, c, ft_strlen(c));
        write(1, "\n", 1);
    }
    return 0;
}
