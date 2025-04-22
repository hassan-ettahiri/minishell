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
    char *c = get_cwd_in_env(e);
    printf("%s\n", c);
    return 0;
}
