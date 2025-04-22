#include "../minishell.h"

int ft_splits(char ***s, char *str)
{
    int i = 0;
    int flag = 0;
    int d = 0;

    *s = ft_malloc(2 * sizeof(char *));
    while (str[i])
    {
        if (str[i] == '+' && str[i + 1] == '=')
        {
            flag = 2;
            d++;
            break ;
        }
        else if (str[i] == '=')
        {
            flag = 1;
            break;
        }
        else
            i++;
    }
    (*s)[0] = ft_substr((char const *)str, 0, i);
    (*s)[1] = ft_substr((char const *)str, i + d +1, ft_strlen(str) - i - 1);
    return flag;
}

t_env *copy_list(t_env *head)
{
    t_env *new_head = NULL;
    t_env *current = head;
    t_env *new_node, *last = NULL;

    while (current)
    {
        new_node = ft_malloc(sizeof(t_env));
        if (!new_node)
            return NULL;
        new_node->key = ft_strdup(current->key);
        new_node->value = ft_strdup(current->value);
        new_node->equl_exist = current->equl_exist;
        new_node->next = NULL;

        if (!new_head)
            new_head = new_node;
        else
            last->next = new_node;

        last = new_node;
        current = current->next;
    }
    return new_head;
}

void ft_swap(t_env *ptr)
{
    char *temp;
    int tmp_num;

    temp = ptr->key;
    ptr->key = ptr->next->key;
    ptr->next->key = temp;
    temp = ptr->value;
    ptr->value = ptr->next->value;
    ptr->next->value = temp;
    tmp_num = ptr->equl_exist;
    ptr->equl_exist = ptr->next->equl_exist;
    ptr->next->equl_exist = tmp_num;
}

void sort_list(t_env *head)
{
    int swapped = 1;
    t_env *ptr;

    if (!head)
        return;

    while (swapped)
    {
        swapped = 0;
        ptr = head;

        while (ptr->next != NULL)
        {
            if (strcmp(ptr->key, ptr->next->key) > 0)
            {
                ft_swap(ptr);
                swapped = 1;
            }
            ptr = ptr->next;
        }
    }
}

int check_special_car(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '!' || str[i] == '@' || str[i] == '#' || str[i] == '$' ||
            str[i] == '%' || str[i] == '^' || str[i] == '&' || str[i] == '*' ||
            str[i] == '(' || str[i] == ')' || str[i] == '+' || str[i] == ':' ||
            str[i] == '?' || str[i] == ',' || str[i] == '.' || str[i] == '/')
            return 1;
        i++;
    }
    return 0;
}

void stack_env(t_env **head, t_env *node, char *env)
{
    char **splits = NULL;
    t_env *tmp = *head;
    node->equl_exist = ft_splits(&splits, env);
    while (tmp)
    {
        if (strcmp(tmp->key, splits[0]) == 0)
        {
            if(node->equl_exist == 2)
            {
                tmp->value = ft_strjoin(tmp->value, splits[1]);
                tmp->equl_exist = 1;
                return ;
            }
            if (node->equl_exist == 1)
            {
                tmp->value = ft_strdup(splits[1]);
                tmp->equl_exist = 1;
                return;
            }
            else
                return;
        }
        tmp = tmp->next;
    }
    if ((ft_isalpha(splits[0][0]) || splits[0][0] == '_') && check_special_car(splits[0]) == 0)
    {
        node->key = ft_strdup(splits[0]);
        node->value = ft_strdup(splits[1]);
        node->next = *head;
        *head = node;
    }
    else
    {
        printf("minishell: export: `%s': not a valid identifier\n", env);
    }
}

int export(t_env **head, char **env, int size)
{
    if (size != 0)
    {
        for (int i = 0; env[i]; i++)
        {
            t_env *node = ft_malloc(sizeof(t_env));
            stack_env(head, node, env[i]);
        }
    }
    else
    {
        t_env *copy = copy_list(*head);
        sort_list(copy);
        t_env *t = copy;
        while (t)
        {
            if (t->equl_exist == 1)
                printf("declare -x %s=\"%s\"\n", t->key, t->value);
            else
                printf("declare -x %s\n", t->key);
            t = t->next;
        }
    }
    return 0;
}
