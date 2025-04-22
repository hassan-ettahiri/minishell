#include "../minishell.h"
char **turn_string_to_double_array(char *str)
{
    char **arr = NULL;

    arr = ft_malloc(sizeof(char *) * 2);
    arr[0] = ft_strdup(str);
    arr[1] = NULL;

    return arr;
}

char *get_env_value(t_env *head, char *str)
{
    t_env *tmp = head;
    while (tmp)
    {
        if (strcmp(tmp->key, str) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

int set_oldpwd(t_env **e, char *prev_pwd)
{
    char *s = ft_strjoin("OLDPWD=", prev_pwd);
    char **env = turn_string_to_double_array(s);
    export(e, env, 1);
    return 0;
}

int set_pwd(t_env **e, char *fallback_path)
{
    char *cwd = getcwd(NULL, 0);
    char *s = NULL;
    if (cwd)
        s = ft_strjoin("PWD=", cwd);
    else
        s = ft_strjoin("PWD=", fallback_path);
    free(cwd);
    char **env = turn_string_to_double_array(s);
    export(e, env, 1);
    return 0;
}

int handle_getcwd_failure(t_env **e, char *str, char *prev_pwd)
{
    printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
    set_oldpwd(e, prev_pwd);
    char *fallback_path = NULL;
    if (strcmp(str, "..") == 0)
        fallback_path = ft_strjoin(prev_pwd, "/..");
    else
        fallback_path = ft_strjoin(prev_pwd, "/.");
    chdir(str);
    set_pwd(e, fallback_path);
    return 0;
}

int add_oldpwd_and_chdir(t_env **e, char *str, int flag)
{
    char *prev_pwd = get_env_value(*e, "PWD");
    if (flag == 1)
        str = get_env_value(*e, "OLDPWD");

    char *cwd = getcwd(NULL, 0);
    if (!cwd && (strcmp(str, "..") == 0 || strcmp(str, ".") == 0))
        return handle_getcwd_failure(e, str, prev_pwd);
    free(cwd);
    if (chdir(str) == 0) {
        set_oldpwd(e, prev_pwd);
        set_pwd(e, NULL);
        return 0;
    } else {
        if (errno == EACCES)
            perror("cd");
        else
            printf("cd: no such file or directory: %s\n", str);
        return 1;
    }
}


int cd(t_env **e, char **str, int size)
{
    if (size > 1)
    {
        printf("minishell: cd: too many arguments");
        return 1;
    }
    else if (size == 0 || (size == 1 && strcmp(str[0], "~") == 0))
        return (add_oldpwd_and_chdir(e, getenv("HOME"), 0));
    else if (size == 1 && strcmp(str[0], "-") == 0)
        return (add_oldpwd_and_chdir(e, NULL, 1));

    return (add_oldpwd_and_chdir(e, str[0], 0));
}
