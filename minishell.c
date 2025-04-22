#include "minishell.h"

void print_array(char **arr)
{
    if (!arr)
    {
        printf("9liwattt f tableau\n");
        return;
    }
    for (size_t i = 0; arr[i]; i++)
    {
        printf("[%s]\n", arr[i]);
    }
}

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

char *strdup_safe(const char *s)
{
    char *d = malloc(strlen(s) + 1);
    if (d)
        strcpy(d, s);
    return d;
}

void trim(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = 0;
}

void parse_input(const char *input, t_pipeline *pipeline)
{
    char *input_copy = strdup_safe(input);
    char *rest = input_copy;
    char *segment;
    int cmd_index = 0;

    while ((segment = strsep(&rest, "|")) != NULL && cmd_index < MAX_COMMANDS)
    {
        trim(segment);

        t_command *cmd = &pipeline->commands[cmd_index];
        char *arg_rest = segment;
        char *arg;
        int param_index = 0;

        // Skip empty tokens until we find the command
        while ((arg = strsep(&arg_rest, " \t")) != NULL)
        {
            if (*arg == '\0')
                continue;
            cmd->command = strdup_safe(arg);
            break;
        }

        // The rest are parameters
        while ((arg = strsep(&arg_rest, " \t")) != NULL)
        {
            if (*arg == '\0')
                continue;
            if (param_index < MAX_ARGS)
            {
                cmd->params[param_index++] = strdup_safe(arg);
            }
        }

        cmd->params[param_index] = NULL;
        cmd->argc = param_index;
        cmd_index++;
    }

    pipeline->count = cmd_index;
    free(input_copy);
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

char *ft_path(int status)
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

char **add_string_on_the_head_of_double_array(char *arr1, char **arr2)
{
    int i;
    int j;

    j = 0;
    i = 0;
    while (arr2[i])
        i++;
    char **new_arr = ft_malloc(sizeof(char *) * (i + 2));
    new_arr[0] = ft_strdup(arr1);
    while (j < i)
    {
        new_arr[j + 1] = ft_strdup(arr2[j]);
        j++;
    }
    new_arr[j + 1] = NULL;
    return new_arr;
}

char *get_path(char *cmd, char **env)
{
    char *path = NULL;
    char *path_env = NULL;
    char *temp = NULL;
    char **paths = NULL;

    if (access(cmd, X_OK) == 0)
    {
        if (ft_strncmp(cmd, "..", 2) == 0 && ft_strlen(cmd) == 2)
        {
            return NULL;
        }
        return ft_strdup(cmd);
    }
    for (int i = 0; env[i]; i++)
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            path_env = env[i] + 5;
            break;
        }
    }

    if (path_env)
    {
        paths = ft_split(path_env, ':');
        for (int i = 0; paths[i]; i++)
        {
            temp = ft_strjoin(paths[i], "/");
            path = ft_strjoin(temp, cmd);
            if (access(path, X_OK) == 0)
                break;
        }
    }
    if (access(path, X_OK) != 0)
    {
        return NULL;
    }
    return path;
}

int ft_execve(char *cmd, char **params, char **env)
{
    int pid = fork();
    char **str = NULL;
    int status;

    if (pid == 0)
    {
        char *path = get_path(cmd, env);
        if (!path)
        {
            fprintf(stderr, "Command not found: %s\n", cmd);
            return 127;
        }
        str = add_string_on_the_head_of_double_array(cmd, params);
        execve(path, str, env);
        perror("execve");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(&status);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        return 1;
    }
}

int ft_execve_with_pipes(char *cmd, char **params, char **env)
{
    char **str = NULL;

    char *path = get_path(cmd, env);
    if (!path)
    {
        fprintf(stderr, "Command not found: %s\n", cmd);
        return 127;
    }
    str = add_string_on_the_head_of_double_array(cmd, params);
    execve(path, str, env);
    perror("execve");
    exit(1);
}

void add_old_pwd(t_env **e)
{
    char **old_pwd = NULL;
    old_pwd = ft_malloc(sizeof(char *) * 2);
    old_pwd[0] = ft_strdup("OLDPWD");
    old_pwd[1] = NULL;
    export(e, old_pwd, 1);
}

int commands(t_env **e, t_pipeline pipe, char **env, int flag)
{
    int status = 0;

    if (pipe.commands[0].command == NULL)
        return 0;
    if (ft_strncmp(pipe.commands[0].command, "env", 3) == 0 && strlen(pipe.commands[0].command) == 3)
        status = ft_env(*e);
    else if (ft_strncmp(pipe.commands[0].command, "export", 6) == 0 && strlen(pipe.commands[0].command) == 6)
        status = export(e, pipe.commands[0].params, pipe.commands[0].argc);
    else if (ft_strncmp(pipe.commands[0].command, "unset", 5) == 0 && strlen(pipe.commands[0].command) == 5)
        status = unset(e, pipe.commands[0].params);
    else if (ft_strncmp(pipe.commands[0].command, "cd", 2) == 0 && strlen(pipe.commands[0].command) == 2)
        status = cd(e, pipe.commands[0].params, pipe.commands[0].argc);
    else if (ft_strncmp(pipe.commands[0].command, "pwd", 3) == 0 && strlen(pipe.commands[0].command) == 3)
        status = pwd(*e);
    // else if (pipe.commands[0].command[0] == '$')
    //     status = dollar(e, cmd, line, env);
    else
    {
        if (flag == -1)
            status = ft_execve(pipe.commands[0].command, pipe.commands[0].params, env);
        else
            status = ft_execve_with_pipes(pipe.commands[flag].command, pipe.commands[flag].params, env);
    }
    return status;
}

void sleeper(void)
{
    int i = 0;
    while (i < SLEEP)
        i++;
}

int main(int ac __attribute__((unused)), char **av __attribute__((unused)), char **env)
{
    char *line = NULL;
    int status = 0;
    write(1, "\033[H\033[J", 6);
    t_pipeline pipe = {0};
    t_env *e = NULL;
    char *s = NULL;
    export(&e, env, 1);
    add_old_pwd(&e);
    while (1)
    {
        sleeper();
        s = ft_path(status);
        line = readline(s);
        if (!line)
        {
            write(1, " exit\n", 6);
            ft_malloc(-1);
            exit(0);
        }
        parse_input(line, &pipe);
        if (ft_strlen(line) != 0)
        {
            add_history(line);
            if (pipe.count == 1)
                commands(&e, pipe, env, -1);
            else
                handel_pipes(&e, pipe, env);
        }
        free(line);
    }
    return 0;
}
