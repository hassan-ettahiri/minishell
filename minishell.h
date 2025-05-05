#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define RED   "\001\033[31m\002->  \001\033[0m\002"
#define GREEN "\001\033[32m\002->  \001\033[0m\002"
#define S_BLUE "\001\033[1;36m\002"
#define E_BLUE "\001\033[0m\002"

typedef enum {
    T_WORD,
    T_PIPE,
    T_SPACE,
    T_REDIR_OUT,
    T_REDIR_IN,
    T_REDIR_OUT_APPEND,
    T_HEREDOC
} TokenType;
typedef struct{
	char	**arr;
	size_t	size;
	size_t	capacity;
} STArray;

typedef struct s_env
{
    char *key;
    char *value;
    int equl_exist;
    struct s_env *next;
} t_env;

typedef struct t_tokens{
	int     length;
	STArray arr;
    STArray category;
    STArray real;
    STArray *TWOD_array_cat;
    STArray *TWOD_array_real;
    STArray *TWOD_array_exp;
    STArray *TWOD_real_deal;
    STArray *TWOD_quoted;
    STArray *TWOD_expanded;
    STArray *TWOD_joined;
    STArray *TWOD_categories;
    STArray *TWOD_new_quoted;
    t_env *e;
	int	wrong;
} t_tokens;



t_tokens 	ft_tokenization(char *s);
void 		ft_init(STArray *dd);
char		*ft_substrr(char *s, int st, int ed);
char		**ft_realloc(STArray *dd, size_t size , size_t cap);
void        ft_tokenization_secondo(t_tokens *tok);
void        ft_splittt(STArray *arr, char *s);
char        **ft_realloc(STArray *dd, size_t size , size_t cap);
void	    ft_clear(STArray *dd);
void        ft_add(STArray *dd, char *s);
void        ft_enumization(t_tokens *tok);
int         ft_space(char c);
void        ft_piping(t_tokens *tok);
void        ft_heredoc(t_tokens* tok, char *s, int k);
int         ft_strcmp(char *s, char *str);
int         ft_strlenn(char *s);
void        ft_check_errors(t_tokens *tok);
int         ft_check(t_tokens *tok, int j);
char        *ft_transform(t_tokens *tok, char *s);
char        *ft_search(t_tokens *tok, char *s);
char        *ft_strjoinn(char *s1, char *s2);
void        ft_expanding(t_tokens *tok);
char        *ft_transform(t_tokens *tok, char *s);
char        *ft_search(t_tokens *tok, char *s);
char        *ft_strjoinn(char *s1, char *s2);
void        ft_joining(t_tokens *tok);

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
#include <ctype.h>

#define MAX_ARGS 10
#define MAX_COMMANDS 10
#define SLEEP 10000000

extern int	flag_sig;

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


typedef struct s_gb
{
    void		*ptr;
    struct s_gb	*next;
}t_gb;

typedef struct {
    char *command;
    char *params[MAX_ARGS + 1];
    STArray infile;
    STArray in;
    STArray outfile;
    STArray out;
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
    int output;
    int in;
    int out;
    int fdt;
    int fds[2];
    int i;
}t_fd;

void sigint_handler(int sig);
void sigint_handler_child(int sig);
void print_array(char **arr);
void	*ft_malloc(ssize_t len);
int cd(t_env **e, char **str, int size);
int export(t_env **head, char **env, int i);
int ft_env(t_env *head);
int unset(t_env **head, char **str);
int echo(char **params, int nb_params);
int exit_status(char** params, int nb_param, int status);
int dollar(t_env **e, t_command cmd, int line, char **env);
int pwd(t_env *e);
char *get_env_value(t_env *head, char *str);
int count_params(char *input);
t_command parse_command(char *input);
char *get_last_dir();
char *ft_path(int status);
char **add_string_on_the_head_of_double_array(char *arr1, char **arr2);
char *get_path(char *cmd, t_env *e);
int open_and_dup(t_command commands, t_fd *fd);
void close_files(t_command commands, t_fd *fd);
int ft_execve(t_pipeline pipe, char **env, t_env e);
int ft_execve_with_pipes(char *cmd, char **params, char **env, t_env e);
void add_old_pwd(t_env **e);
int commands(t_env **e, t_pipeline pipe, char **env, int flag);
int handel_pipes(t_env **e, t_pipeline pipe, char **env);
#endif


// cat < Makefile < piping.c < heredoc.c <main.c 
// ls > test1.txt > test2.txt >> test3.txt > test4.txt
// cat < Makefile  | grep p > test.txt