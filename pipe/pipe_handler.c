#include "../minishell.h"

// void redfirst(t_fd *fd)
// {
// 	fd->in = dup(STDIN_FILENO);
// 	if (pipe(fd->fds) == -1)
// 		exit(1);
// 	fd->out = fd->fds[1];
// 	fd->fdt = fd->fds[0];
// }

// void rbetween(t_fd *fd)
// {
// 	fd->in = fd->fdt;
// 	if (pipe(fd->fds) == -1)
// 		exit(1);
// 	fd->out = fd->fds[1];
// 	fd->fdt = fd->fds[0];
// }

// void redlast(t_fd *fd)
// {
// 	fd->in = fd->fdt;
// 	fd->out = open("/dev/stdout", O_WRONLY);
// }

int open_and_dup_pipes(t_command commands, t_fd *fd)
{
	int i;

	if (commands.infile.size > 0)
	{
		for (i = 0; i < commands.infile.size - 1; i++)
		{
			int tmp_fd;
			if (commands.in.arr[i][0] == '-')
				tmp_fd = open(commands.infile.arr[i], O_RDONLY);
			else
				tmp_fd = ft_atoi(commands.infile.arr[i]);

			if (tmp_fd == -1)
			{
				printf("%s: No such file or directory\n", commands.infile.arr[i]);
				return (1);
			}
			close(tmp_fd);
		}

		i = commands.infile.size - 1;
		if (commands.in.arr[i][0] == '-')
			fd->in = open(commands.infile.arr[i], O_RDONLY);
		else
			fd->in = ft_atoi(commands.infile.arr[i]);

		if (fd->in == -1)
		{
			printf("%s: No such file or directory\n", commands.infile.arr[i]);
			return (1);
		}
		dup2(fd->in, STDIN_FILENO);
		close(fd->in);
	}
	if (commands.outfile.size > 0)
	{
		for (i = 0; i < commands.outfile.size - 1; i++)
		{
			int tmp_fd;
			if (commands.out.arr[i][0] == '+')
				tmp_fd = open(commands.outfile.arr[i], O_WRONLY | O_CREAT | O_APPEND, 0777);
			else
				tmp_fd = open(commands.outfile.arr[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);

			if (tmp_fd == -1)
			{
				printf("%s: No such file or directory\n", commands.outfile.arr[i]);
				return (1);
			}
			close(tmp_fd);
		}

		i = commands.outfile.size - 1;
		if (commands.out.arr[i][0] == '+')
			fd->out = open(commands.outfile.arr[i], O_WRONLY | O_CREAT | O_APPEND, 0777);
		else
			fd->out = open(commands.outfile.arr[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);

		if (fd->in == -1)
		{
			printf("%s: No such file or directory\n", commands.outfile.arr[i]);
			return (1);
		}
		dup2(fd->out, STDOUT_FILENO);
		close(fd->out);
	}
	return 0;
}

void close_files_pipes(t_command commands, t_fd *fd)
{
	if (commands.infile.size > 0 && fd->input >= 0)
	{
		dup2(fd->input, STDIN_FILENO);
		close(fd->input);
	}
	if (commands.outfile.size > 0 && fd->output >= 0)
	{
		dup2(fd->output, STDOUT_FILENO);
		close(fd->output);
	}
}

void pipe_execution(t_fd *fd, t_pipeline pipe, char **env, t_env **e)
{
	int status;

	if (fd->fdt != -1)
	{
		if (pipe.commands[fd->i].infile.size == 0 && pipe.commands[fd->i - 1].outfile.size == 0)
		{
			dup2(fd->fdt, STDIN_FILENO);
			close(fd->fdt);
		}
		else
		{
			if (pipe.commands[fd->i].infile.size > 0)
			{
				close(fd->fdt);
				dup2(fd->in, STDIN_FILENO);
				close(fd->in);
			}
			else
			{
				dup2(fd->fdt, STDIN_FILENO);
				close(fd->fdt);
			}
		}
	}
	if (fd->i < pipe.count - 1)
	{
		if (pipe.commands[fd->i].outfile.size == 0)
		{
			close(fd->fds[0]);
			dup2(fd->fds[1], STDOUT_FILENO);
			close(fd->fds[1]);
		}
		else
		{
			close(fd->fds[0]);
			close(fd->fds[1]);
			dup2(fd->out, STDOUT_FILENO);
			close(fd->out);
		}
	}
	status = commands(e, pipe, env, fd->i);
	exit(status);
}

int handel_pipes(t_env **e, t_pipeline pipel, char **env)
{
	t_fd fd;
	t_fd red;
	pid_t *child;
	int i;

	fd.i = 0;
	fd.fdt = -1;
	fd.input = dup(STDIN_FILENO);
	fd.output = dup(STDOUT_FILENO);

	child = ft_malloc(sizeof(pid_t) * pipel.count);
	if (!child)
		return (1);

	while (fd.i < pipel.count)
	{
		if (fd.i < pipel.count - 1)
		{
			if (pipe(fd.fds) == -1)
			{
				perror("pipe");
				return (1);
			}
		}
		child[fd.i] = fork();
		if (child[fd.i] < 0)
		{
			perror("fork");
			return (1);
		}
		else if (child[fd.i] == 0)
		{
			flag_sig = 0;
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			if (open_and_dup_pipes(pipel.commands[fd.i], &fd) == 1)
				exit(1);
			pipe_execution(&fd, pipel, env, e);
		}
		else
		{
			if (fd.fdt != -1)
				close(fd.fdt);
			if (fd.i < pipel.count - 1)
			{
				close(fd.fds[1]);
				if (pipel.commands[fd.i + 1].in.size == 0)
					fd.fdt = fd.fds[0];
				else
				{
					close(fd.fdt);
					close(fd.fds[0]);
				}
			}
		}
		fd.i++;
	}
	if (fd.fdt != -1)
		close(fd.fdt);
	for (i = 0; i < pipel.count; i++)
	{
		waitpid(child[i], &pipel.status, 0);
		if (WIFSIGNALED(pipel.status))
		{
			if (WTERMSIG(pipel.status) == SIGQUIT)
			{
				pipel.status = 131;
			}
			else if (WTERMSIG(pipel.status) == SIGINT)
			{
				pipel.status = 130;
			}
		}
		if (WIFEXITED(pipel.status))
		{
			pipel.status = WEXITSTATUS(pipel.status);
		}
	}
	dup2(fd.input, STDIN_FILENO);
	close(fd.input);
	dup2(fd.output, STDOUT_FILENO);
	close(fd.output);
	return pipel.status;
}

/*
	int in;
	int out;
	int fdt;
	int fds[2];

		(parent)
		pipe(fds);
		in = infile
		out = fd[0];
		fdt = fd[1];

		(i == 0)
		-> 1st iteration
			fork()
			child ->
				dup (in) into 1
				dup (out) into 0
				close the dupped fds
				and close fdt

		(parent)
		pipe(fds)
		in = fdt
		out = fds[0]
		fdt = fds[1]

		2nd iteration
			fork()
			child ->
				dup (in) into 1
				dup (out) into 0
				close the dupped fds
				and close fdt

		[if there is more pipes, we keep going the same way]
		[if the next pipe is the last (i == number of pipes)]

		(parent)
		in = fdt
		out = outfile

		-> last iteration
			fork()
			child ->
				dup (in) into 1
				dup (out) into 0
				close the dupped fds
				and close fdt

		(parent)
			close (fdt)
			wait()
*/