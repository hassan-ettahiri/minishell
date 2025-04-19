#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

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

typedef struct
{
    char *command;
    char **params;
} t_command;

void	*ft_malloc(ssize_t len);
int cd(char **str, int size);
int export(t_env **head, char **env, int i);
int ft_env(t_env *head);
int unset(t_env **head, char **str);
int pwd();

#endif