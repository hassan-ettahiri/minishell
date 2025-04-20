#include "../minishell.h"
char **turn_string_to_double_array(char *str)
{
    char **arr = NULL;
    int i = 0;
    int j = 0;
    int k = 0;

    arr = ft_malloc(sizeof(char *) * 2);
    arr[0] = ft_strdup(str);
    arr[1] = NULL;
    
    return arr;
}

char *get_old_pwd(t_env *head)
{
    t_env *tmp = head;
    while (tmp)
    {
        if (strcmp(tmp->key, "OLDPWD") == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

int cd(t_env **e, char **str, int size)
{
    if(size > 1){
        printf("minishell: cd: too many arguments");
        return 1;
    }else if(size == 0 || (size == 1 && strcmp(str[0], "~") == 0)){
        char *oldpwd = getcwd(NULL, 0);
        char *s = ft_strjoin("OLDPWD=", oldpwd);
        free(oldpwd);
        char **env = turn_string_to_double_array(s);
        if (chdir(getenv("HOME")) == 0)
        {
            export(e, env, 1);
            return 0;
        }
        printf("cd: no such file or directory: %s\n", getenv("HOME"));
        return 1;
    }
    else if(size == 1 && strcmp(str[0], "-") == 0){
        char *oldpwd = getcwd(NULL, 0);
        char *s = ft_strjoin("OLDPWD=", oldpwd);
        free(oldpwd);
        char **env = turn_string_to_double_array(s);
        oldpwd = get_old_pwd(*e);
        if (chdir(oldpwd) == 0){
            export(e, env, 1);
            return 0;
        }
        printf("cd: no such file or directory: %s\n", oldpwd);
        return 1;
    }
    char *oldpwd = getcwd(NULL, 0);
    char *s = ft_strjoin("OLDPWD=", oldpwd);
    free(oldpwd);
    char **env = turn_string_to_double_array(s);
    if (chdir(str[0]) == 0){
        export(e, env, 1);
        return 0;
    }
    printf("cd: no such file or directory: %s\n", str[0]);
    return 1;
}