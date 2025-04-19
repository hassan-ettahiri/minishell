#include "../minishell.h"

int unset(t_env **head, char **str)
{
    t_env *tmp;
    t_env *p;
    int i;

    i = 0;
    while (str[i])
    {
        while (*head && strcmp((*head)->key, str[i]) == 0)
        {
            p = *head;
            *head = (*head)->next;
        }

        tmp = *head;
        while (tmp && tmp->next)
        {
            if (strcmp(tmp->next->key, str[i]) == 0)
            {
                p = tmp->next;
                tmp->next = tmp->next->next;
            }
            tmp = tmp->next;
        }
        i++;
    }
    return 0;
}
