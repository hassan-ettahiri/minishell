#include "minishell.h"

void ft_merge(t_tokens *tok, t_pipeline *pipe)
{
	pipe->count = tok->length;
	int i = 0;
	int k = 0;

	while (i < tok->length)
	{
		size_t j = 0;
		ft_init(&pipe->commands[i].infile);
		ft_init(&pipe->commands[i].outfile);
		ft_init(&pipe->commands[i].in);
		ft_init(&pipe->commands[i].out);
		pipe->commands[i].argc = 0;
		k = 0;
		while (j < tok->TWOD_joined[i].size)
		{
			if (tok->TWOD_categories[i].arr[j][1] == 'O')
			{
				if (tok->TWOD_categories[i].arr[j + 1][1] == 'S')
				{
					ft_add(&pipe->commands[i].outfile, tok->TWOD_joined[i].arr[j + 2]);
					j += 2;
				}
				else
				{
					ft_add(&pipe->commands[i].outfile, tok->TWOD_joined[i].arr[j + 1]);
					j++;
				}
				ft_add(&pipe->commands[i].out, ft_substrr("-", 0, 0));
				j++;
				continue;
			}
			if (tok->TWOD_categories[i].arr[j][1] == 'I')
			{
				if (tok->TWOD_categories[i].arr[j + 1][1] == 'S')
				{
					ft_add(&pipe->commands[i].infile, tok->TWOD_joined[i].arr[j + 2]);
					j += 2;
				}
				else
				{
					ft_add(&pipe->commands[i].infile, tok->TWOD_joined[i].arr[j + 1]);
					j++;
				}
				ft_add(&pipe->commands[i].in, ft_substrr("-", 0, 0));
				j++;
				continue;
			}
			if (tok->TWOD_categories[i].arr[j][1] == 'H')
			{
				if (tok->TWOD_categories[i].arr[j + 1][1] == 'S')
				{
					ft_add(&pipe->commands[i].infile, tok->TWOD_joined[i].arr[j + 2]);
					j += 2;
				}
				else
				{
					ft_add(&pipe->commands[i].infile, tok->TWOD_joined[i].arr[j + 1]);
					j++;
				}
				ft_add(&pipe->commands[i].in, ft_substrr("+", 0, 0));
				j++;
				continue;
			}
			if (tok->TWOD_categories[i].arr[j][1] == 'A')
			{
				if (tok->TWOD_categories[i].arr[j + 1][1] == 'S')
				{
					ft_add(&pipe->commands[i].outfile, tok->TWOD_joined[i].arr[j + 2]);
					j += 2;
				}
				else
				{
					ft_add(&pipe->commands[i].outfile, tok->TWOD_joined[i].arr[j + 1]);
					j++;
				}
				ft_add(&pipe->commands[i].out, ft_substrr("+", 0, 0));
				j++;
				continue;
			}
			if (tok->TWOD_categories[i].arr[j][1] == 'S')
			{
				j++;
				continue;
			}
			if (k == 0)
				pipe->commands[i].command = tok->TWOD_joined[i].arr[j];
			else
				pipe->commands[i].params[pipe->commands[i].argc] = tok->TWOD_joined[i].arr[j];
			if (k != 0)
				pipe->commands[i].argc++;
			k = 1;
			j++;
		}
		// j = 0;
		// while (j < pipe->commands[i].infile.size)
		// {
		// 	printf("%s %s: infile\n", pipe->commands[i].infile.arr[j], pipe->commands[i].in.arr[j]);
		// 	j++;
		// }
		// j = 0;
		// while (j < pipe->commands[i].outfile.size)
		// {
		// 	printf("%s %s: outfile\n", pipe->commands[i].outfile.arr[j], pipe->commands[i].out.arr[j]);
		// 	j++;
		// }
		i++;
	}
}

int flag_sig = 0;

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

char *get_last_dir()
{
	char *cwd = getcwd(NULL, 0);
	if (!cwd)
		return NULL;

	char *last_slash = ft_strrchr(cwd, '/');
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
	s.temp1 = ft_strjoin(S_BLUE, s.cwd);
	s.temp2 = ft_strjoin(s.temp1, E_BLUE);
	s.temp3 = ft_strjoin(s.temp2, " ");
	if (status == 0)
		s.prompt = ft_strjoin(GREEN, s.temp3);
	else
		s.prompt = ft_strjoin(RED, s.temp3);
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

char *get_path(char *cmd, t_env *e)
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
	for (t_env *t = e; t; t = t->next)
	{
		if (ft_strncmp(t->key, "PATH", 4) == 0 && ft_strlen(t->key) == 4)
		{
			path_env = ft_strdup(t->value);
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
void handler_quit(int sig)
{
	if (sig == SIGQUIT)
	{
		exit(131);
	}
}

int ft_execve(t_pipeline pipe, char **env, t_env e)
{
	pid_t pid;
	char *sh_argv[3];
	char **str;

	pid = fork();
	if (pid == 0)
	{
		flag_sig = 0;
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (ft_strncmp(pipe.commands[0].command, "./", 2) == 0 || pipe.commands[0].command[0] == '/')
		{
			if (access(pipe.commands[0].command, X_OK) == 0)
			{
				execve(pipe.commands[0].command, pipe.commands[0].params, env);
			}
			else
			{
				fprintf(stderr, "bash: %s: No such file or directory\n", pipe.commands[0].command);
				exit(127);
			}
		}
		char *path = get_path(pipe.commands[0].command, &e);
		if (!path)
		{
			fprintf(stderr, "bash: command not found: %s\n", pipe.commands[0].command);
			exit(127);
		}
		str = add_string_on_the_head_of_double_array(pipe.commands[0].command, pipe.commands[0].params);
		execve(path, str, env);
		if (errno == ENOEXEC)
		{
			sh_argv[0] = "/bin/sh";
			sh_argv[1] = pipe.commands[0].command;
			sh_argv[2] = NULL;
			execve("/bin/sh", sh_argv, env);
		}
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
		int status;
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				return 131;
			}
			else if (WTERMSIG(status) == SIGINT)
			{
				write(1, "\n", 1);
				return 130;
			}
		}
		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		return 1;
	}
}

int ft_execve_with_pipes(char *cmd, char **params, char **env, t_env e)
{
	char **str = NULL;

	if (ft_strncmp(cmd, "./", 2) == 0 || cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
		{
		}
		else
		{
			printf("bash: %s: No such file or directory\n", cmd);
			return 127;
		}
	}
	char *path = get_path(cmd, &e);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd);
		return 127;
	}
	str = add_string_on_the_head_of_double_array(cmd, params);
	execve(path, str, env);
	perror("execve");
	return 0;
}

void add_old_pwd(t_env **e)
{
	char **old_pwd = NULL;
	old_pwd = ft_malloc(sizeof(char *) * 2);
	old_pwd[0] = ft_strdup("OLDPWD");
	old_pwd[1] = NULL;
	export(e, old_pwd, 1);
}

int open_and_dup(t_command commands, t_fd *fd)
{
	int i;

	if (commands.infile.size != 0)
	{
		fd->input = dup(STDIN_FILENO);
		i = 0;
		while (i < commands.infile.size)
		{
			if (commands.in.arr[i][0] == '-')
			{
				fd->in = open(commands.infile.arr[i], O_RDONLY);
				if (fd->in == -1)
				{
					printf("%s: No such file or directory\n", commands.infile.arr[i]);
					dup2(fd->in, STDIN_FILENO);
					close(fd->in);
					return 1;
				}
			}
			else
			{
				fd->in = ft_atoi(commands.infile.arr[i]);
				if (fd->in == -1)
				{
					printf("heredoc: No such file or directory\n");
					dup2(fd->in, STDIN_FILENO);
					close(fd->in);
					return 1;
				}
			}
			i++;
		}
	}
	if (commands.outfile.size != 0)
	{
		fd->output = dup(STDOUT_FILENO);
		i = 0;
		while (i < commands.outfile.size)
		{
			if (commands.out.arr[i][0] == '+')
			{
				fd->out = open(commands.outfile.arr[i], O_WRONLY | O_CREAT | O_APPEND, 0777);
				if (fd->in == -1)
				{
					printf("%s: No such file or directory\n", commands.outfile.arr[i]);
					dup2(fd->out, STDOUT_FILENO);
					close(fd->in);
					return 1;
				}
			}
			else
			{
				fd->out = open(commands.outfile.arr[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
				if (fd->in == -1)
				{
					printf("%s: No such file or directory\n", commands.outfile.arr[i]);
					dup2(fd->out, STDOUT_FILENO);
					close(fd->in);
					return 1;
				}
			}
			i++;
		}
	}
	return 0;
}

void close_files(t_command commands, t_fd *fd)
{
	if (commands.infile.size != 0)
	{
		dup2(fd->input, STDIN_FILENO);
		close(fd->input);
	}
	if (commands.outfile.size != 0)
	{
		dup2(fd->output, STDOUT_FILENO);
		close(fd->output);
	}
}

int commands(t_env **e, t_pipeline pipe, char **env, int flag)
{
	int status = 0;
	t_fd red;
	if (flag == -1)
	{
		status = open_and_dup(pipe.commands[0], &red);
		if (status == 1)
			return 1;
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
		else if (ft_strncmp(pipe.commands[0].command, "echo", 4) == 0 && strlen(pipe.commands[0].command) == 4)
			status = echo(pipe.commands[0].params, pipe.commands[0].argc);
		else if (ft_strncmp(pipe.commands[0].command, "exit", 4) == 0 && strlen(pipe.commands[0].command) == 4)
			exit_status(pipe.commands[0].params, pipe.commands[0].argc, pipe.status);
		else
			status = ft_execve(pipe, env, **e);
		close_files(pipe.commands[0], &red);
	}
	else
	{
		if (pipe.commands[flag].command == NULL)
			return 0;
		if (ft_strncmp(pipe.commands[flag].command, "env", 3) == 0 && strlen(pipe.commands[flag].command) == 3)
			status = ft_env(*e);
		else if (ft_strncmp(pipe.commands[flag].command, "export", 6) == 0 && strlen(pipe.commands[flag].command) == 6)
			status = export(e, pipe.commands[flag].params, pipe.commands[flag].argc);
		else if (ft_strncmp(pipe.commands[flag].command, "unset", 5) == 0 && strlen(pipe.commands[flag].command) == 5)
			status = unset(e, pipe.commands[flag].params);
		else if (ft_strncmp(pipe.commands[flag].command, "cd", 2) == 0 && strlen(pipe.commands[flag].command) == 2)
			status = cd(e, pipe.commands[flag].params, pipe.commands[flag].argc);
		else if (ft_strncmp(pipe.commands[flag].command, "pwd", 3) == 0 && strlen(pipe.commands[flag].command) == 3)
			status = pwd(*e);
		else if (ft_strncmp(pipe.commands[flag].command, "echo", 4) == 0 && strlen(pipe.commands[flag].command) == 4)
			status = echo(pipe.commands[flag].params, pipe.commands[flag].argc);
		else if (ft_strncmp(pipe.commands[flag].command, "exit", 4) == 0 && strlen(pipe.commands[flag].command) == 4)
			status = exit_status(pipe.commands[flag].params, pipe.commands[flag].argc, pipe.status);
		else
		{
			status = ft_execve_with_pipes(pipe.commands[flag].command, pipe.commands[flag].params, env, **e);
		}
	}
	return status;
}

void sleeper(void)
{
	int i = 0;
	while (i < SLEEP)
		i++;
}

char **set_defaults(t_env **e, int flag)
{
	char **defaults;
	char *getpwd;

	defaults = ft_malloc(4 * sizeof(char *));
	if (!defaults)
		return NULL;

	defaults[0] = ft_strdup("PATH=/virtualbox/bin:/virtualbox/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");

	getpwd = getcwd(NULL, 0);
	if (!getpwd)
		getpwd = ft_strdup("/unknown");

	defaults[1] = ft_strjoin("PWD=", getpwd);
	free(getpwd);

	defaults[2] = ft_strdup("SHLVL=1");
	defaults[3] = NULL;

	export(e, defaults, flag);

	return defaults;
}

char **ft_copy_env(char **env)
{
	int i = 0;
	char **copy;

	if (env == NULL)
		return NULL;
	while (env[i])
		i++;

	copy = ft_malloc((i + 1) * sizeof(char *));
	if (!copy)
		return NULL;

	i = 0;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	return copy;
}

char **ft_export(t_env **e, char **env, int flag)
{
	char **str;
	export(e, env, flag);
	str = ft_copy_env(env);
	return str;
}

void free_STArray(STArray *array)
{
	if (!array || !array->arr)
		return;
	for (size_t i = 0; i < array->size; ++i)
	{
		array->arr[i] = NULL;
	}
	array->arr = NULL;
	array->size = 0;
	array->capacity = 0;
}

int main(int ac __attribute__((unused)), char **av __attribute__((unused)), char **env)
{
	char *line = NULL;
	flag_sig = 0;
	write(1, "\033[H\033[J", 6);
	t_pipeline pipe = {0};
	t_env *e = NULL;
	char **copy_env;
	char *s = NULL;
	int l = 0;
	t_tokens tok;
	pipe.status = 0;
	if (env[0] != NULL)
		copy_env = ft_export(&e, env, 1);
	else
		copy_env = set_defaults(&e, 1);
	add_old_pwd(&e);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (isatty(0) != 1)
			return 0;
		flag_sig = 1;
		sleeper();
		s = ft_path(pipe.status);
		line = readline(s);
		add_history(line);
		flag_sig = 0;
		if (!line)
		{
			write(1, " exit\n", 6);
			ft_malloc(-1);
			exit(0);
		}
		tok = ft_tokenization(line);
		if (tok.wrong)
		{
			printf("ERROR\n");
			pipe.status = 1;
			continue;
		}
		ft_enumization(&tok);
		if (tok.wrong)
		{
			printf("ERROR\n");
			pipe.status = 1;
			continue;
		}
		ft_piping(&tok);

		tok.e = e;
		ft_expanding(&tok);
		ft_joining(&tok);
		ft_check_errors(&tok);
		if (tok.wrong)
		{
			printf("ERROR\n");
			pipe.status = 1;
			continue;
		}
		ft_merge(&tok, &pipe);
		int kk = 0;
		if (ft_strlen(line) != 0)
		{
			if (pipe.count == 1)
				pipe.status = commands(&e, pipe, copy_env, -1);
			else
				pipe.status = handel_pipes(&e, pipe, copy_env);
		}
		printf("\nstatus: %d\n", pipe.status);
		l = 0;
		while (l < pipe.count)
		{
			pipe.commands[l].command = NULL;
			int j = 0;
			while (j < pipe.commands[l].argc)
			{
				pipe.commands[l].params[j] = NULL;
				j++;
			}
			l++;
		}
	}
	return 0;
}
