#include "../minishell.h"

void redfirst(t_fd *fd)
{
    fd->in = open("/dev/stdin", O_RDONLY, 0777);
    if (fd->in == -1)
        exit(1);
    
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
    fd->out = open("/dev/stdout", O_WRONLY, 0777);
}

void pipe_execution(t_fd *fd, t_pipeline pipe, char **env, t_env **e)
{
    if(dup2(fd->in, STDIN_FILENO) == -1)
        dprintf(2, "failed to dup 1\n");
    if (dup2(fd->out, STDOUT_FILENO) == -1)
        dprintf(2, "failed to dup 2\n");
    close(fd->in);
    close(fd->out);
    commands(e, pipe, env, fd->i);
}

void handel_pipes(t_env **e, t_pipeline pipe, char **env)
{
    t_fd fd;
    pid_t child;
    fd.i = 0;
    fd.in = dup(0);
    fd.out = dup(1);

    while (fd.i < pipe.count)
    {
        if (fd.i == 0)
            redfirst(&fd);
        else if (fd.i == pipe.count - 1)
            redlast(&fd);
        else
            rbetween(&fd);
        child = fork();
        if (child == 0)
            pipe_execution(&fd, pipe, env, e);
        
        if (fd.i != 0)
            close(fd.in);
        if (fd.i != pipe.count - 1)
            close(fd.out);
        fd.i++;
    }
    while( wait(NULL) == -1)
        ;
    close(fd.fdt);
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