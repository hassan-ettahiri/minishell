#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <errno.h>
#include <signal.h>

#define MAX_ARGS 10
#define MAX_COMMANDS 10
#define SLEEP 10000000

typedef struct s_design
{
    char *color_start;
    char *color_end;
    char *cwd;
    char *temp;
    char *temp1;
    char *temp2;
    char *temp3;
    char *prompt;
} t_design;

typedef struct s_env
{
    char *key;
    char *value;
    int equl_exist;
    struct s_env *next;
} t_env;

typedef struct s_gb
{
    void		*ptr;
    struct s_gb	*next;
}t_gb;

typedef struct {
    char *command;
    char *params[MAX_ARGS + 1];
    int argc;
} t_command;

typedef struct {
    t_command commands[MAX_COMMANDS];
    int count;
    int status;
} t_pipeline;
	
typedef struct
{
    int input;
    int in;
    int out;
    int fdt;
    int fds[2];
    int i;
}t_fd;


void print_array(char **arr);
void	*ft_malloc(ssize_t len);
int cd(t_env **e, char **str, int size);
int export(t_env **head, char **env, int i);
int ft_env(t_env *head);
int unset(t_env **head, char **str);
int dollar(t_env **e, t_command cmd, int line, char **env);
int pwd(t_env *e);
char *get_env_value(t_env *head, char *str);
int count_params(char *input);
t_command parse_command(char *input);
char *get_last_dir();
char *ft_path(int status);
char **add_string_on_the_head_of_double_array(char *arr1, char **arr2);
char *get_path(char *cmd, t_env *e);
int ft_execve(char *cmd, char **params, char **env, t_env e);
int ft_execve_with_pipes(char *cmd, char **params, char **env, t_env e);
void add_old_pwd(t_env **e);
int commands(t_env **e, t_pipeline pipe, char **env, int flag);
int handel_pipes(t_env **e, t_pipeline pipe, char **env);

#endif