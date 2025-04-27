#include "../minishell.h"

void redfirst(t_fd *fd)
{
	fd->in = dup(STDIN_FILENO);
	if (pipe(fd->fds) == -1)
		exit(1);
	fd->out = fd->fds[1];
	fd->fdt = fd->fds[0];
}

void rbetween(t_fd *fd)
{
	fd->in = fd->fdt;
	if (pipe(fd->fds) == -1)
		exit(1);
	fd->out = fd->fds[1];
	fd->fdt = fd->fds[0];
}

void redlast(t_fd *fd)
{
	fd->in = fd->fdt;
	fd->out = open("/dev/stdout", O_WRONLY);
}

void pipe_execution(t_fd *fd, t_pipeline pipe, char **env, t_env **e)
{
	int status;
	fd->out = STDOUT_FILENO;
	fd->in = STDIN_FILENO;
	if (fd->fdt != -1)
	{
		dup2(fd->fdt, STDIN_FILENO);
		close(fd->fdt);
		fd->in = fd->fdt;
	}
	if (fd->i < pipe.count - 1)
	{
		close(fd->fds[0]);
		dup2(fd->fds[1], STDOUT_FILENO);
		close(fd->fds[1]);
		fd->out = fd->fds[1];
	}
	if (fd->in != STDIN_FILENO)
	{
		dup2(fd->in, STDIN_FILENO);
		close(fd->in);
	}
	if (fd->out != STDOUT_FILENO)
	{
		dup2(fd->out, STDOUT_FILENO);
		close(fd->out);
	}
	status = commands(e, pipe, env, fd->i);
	exit(status);
}

int handel_pipes(t_env **e, t_pipeline pipel, char **env)
{
	t_fd fd;
	pid_t *child = ft_malloc(sizeof(pid_t) * pipel.count);
	if (!child)
		return (1);
	fd.i = 0;
	fd.fdt = -1;
	int i = 0;
	fd.input = dup(STDIN_FILENO);
	while (fd.i < pipel.count)
	{
		if (i < pipel.count - 1)
		{
			if (pipe(fd.fds) == -1)
			{
				perror("pipe");
				return 1;
			}
		}
		child[fd.i] = fork();
		if (child[fd.i] == 0)
		{
			flag_sig = 0;
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			pipe_execution(&fd, pipel, env, e);
		}
		else
		{
			if (fd.fdt != -1)
				close(fd.fdt);
			if (fd.i < pipel.count - 1)
			{
				close(fd.fds[1]);
				fd.fdt = fd.fds[0];
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
				write(1, "Quit (core dumped)\n", 20);
				return 131;
			}
			else if (WTERMSIG(pipel.status) == SIGINT)
			{
				write(1, "\n", 1);
				return 130;
			}
		}
		if (WIFEXITED(pipel.status))
			pipel.status = WEXITSTATUS(pipel.status);
	}
	dup2(fd.input, STDIN_FILENO);
	close(fd.input);
	return 0;
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