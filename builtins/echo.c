#include "../minishell.h"

int echo(char **params, int nb_params)
{
    int i = 0;
    int flag = 0;

    if (strcmp(params[i], "-n") == 0)
    {
        flag = 1;
        i++;
    }

    while (params[i])
    {
        printf("%s", params[i]);
        if (i < nb_params)
        {
            printf(" ");
        }
        i++;
    }
    if (flag == 0)
    {
        printf("\n");
    }
    return 0;
}