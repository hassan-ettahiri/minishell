#include "minishell.h"

int count_params(char *input)
{
    int count = 0;
    char *tmp = ft_strdup(input);
    char *token = strtok(tmp, " ");
    token = strtok(NULL, " ");

    while (token)
    {
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}

t_command parse_command(char *input)
{
    t_command cmd;
    cmd.command = NULL;
    cmd.params = NULL;

    char *copy = ft_strdup(input);
    if (!copy)
        return cmd;

    int count = 0;
    char *token = strtok(copy, " ");
    if (!token)
    {
        return cmd;
    }

    cmd.command = ft_strdup(token);

    char *param_start = copy + ft_strlen(token) + 1;
    char *param_copy = ft_strdup(param_start);
    if (!param_copy)
    {
        return cmd;
    }

    token = strtok(param_copy, " ");
    while (token)
    {
        count++;
        token = strtok(NULL, " ");
    }

    cmd.params = ft_malloc(sizeof(char *) * (count + 1));
    int i = 0;
    char *param_copy_1 = ft_strdup(param_start);
    token = strtok(param_copy_1, " ");
    while (token)
    {
        cmd.params[i++] = ft_strdup(token);
        token = strtok(NULL, " ");
    }
    cmd.params[i] = NULL;

    return cmd;
}

char *get_last_dir()
{
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
        return NULL;

    char *last_slash = strrchr(cwd, '/');
    char *last_dir;

    if (last_slash && *(last_slash + 1) != '\0')
        last_dir = ft_strdup(last_slash + 1);
    else
        last_dir = ft_strdup("/");
    free(cwd);
    return last_dir;
}

char* ft_path(int status)
{
    t_design s;

    s.cwd = get_last_dir();
    s.color_start = "\001\033[1;36m\002";
    s.color_end = "\001\033[0m\002";
    if (status == 0)
        s.temp = ft_strdup("\033[32m➜  \033[0m");
    else
        s.temp = ft_strdup("\033[31m➜  \033[0m");
    s.temp1 = ft_strjoin(s.color_start, s.cwd);
    s.temp2 = ft_strjoin(s.temp1, s.color_end);
    s.temp3 = ft_strjoin(s.temp2, " ");
    s.prompt = ft_strjoin(s.temp, s.temp3);
    return s.prompt;
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    char *line = NULL;
    int status = 0;
    write(1, "\033[H\033[J", 6);
    t_command cmd;
    cmd.command = NULL;
    cmd.params = NULL;
    t_env *e = NULL;
    export(&e, env, 1);
    char *s;
    while (1)
    {
        s = ft_path(status);
        line = readline(s);
        if (!line)
            break;
        cmd = parse_command(line);
        if (cmd.command == NULL)
            continue;
        if (ft_strncmp(cmd.command, "env", 3) == 0 && strlen(cmd.command) == 3)
        {
            status = ft_env(e);
        }
        else if (ft_strncmp(cmd.command, "export", 6) == 0 && strlen(cmd.command) == 6)
        {
            status = export(&e, cmd.params, count_params(line));
        }
        else if (ft_strncmp(cmd.command, "unset", 5) == 0 && strlen(cmd.command) == 5)
        {
            status = unset(&e, cmd.params);
        }
        else if (ft_strncmp(cmd.command, "cd", 2) == 0 && strlen(cmd.command) == 2)
        {
            status = cd(cmd.params, count_params(line));
        }
        else if (ft_strncmp(cmd.command, "pwd", 3) == 0 && strlen(cmd.command) == 3)
        {
            status = pwd();
        }
        else
            status = 1;
    }
    return 0;
}
