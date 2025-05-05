#include "../minishell.h"

int unset(t_env **head, char **str)
{
    t_env *tmp;
    int i;

    i = 0;
    while (str[i])
    {
        while (*head && strcmp((*head)->key, str[i]) == 0)
        {
            *head = (*head)->next;
        }

        tmp = *head;
        while (tmp && tmp->next)
        {
            if (strcmp(tmp->next->key, str[i]) == 0)
            {
                tmp->next = tmp->next->next;
            }
            tmp = tmp->next;
        }
        i++;
    }
    return 0;
}
