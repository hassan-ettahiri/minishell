#include "minishell.h"

void ft_enumization(t_tokens *tok)
{
	size_t	i;
	STArray new_one;
	STArray new_second;
	ft_init(&new_one);
	ft_init(&new_second);
	i = 0;
	while(i < tok->arr.size)
	{
		if(ft_space(tok->arr.arr[i][0]))
		{
			ft_add(&new_one, "[SPACE]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		else if(tok->arr.arr[i][0] == '\"')
		{
			ft_add(&new_one, "[COMMAND]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		else if(tok->arr.arr[i][0] == '\'')
		{
			ft_add(&new_one, "[COMMAND]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		else if(tok->arr.arr[i][0] == '|')
		{
			ft_add(&new_one, "[PIPE]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		else if(tok->arr.arr[i][0] == '<' && i + 1 < tok->arr.size && tok->arr.arr[i+1][0] == '<')
		{
			ft_add(&new_one, "[HEREDOC]");
			ft_add(&new_second, "<<");
			i++;
		}
		else if(tok->arr.arr[i][0] == '>' && i + 1 < tok->arr.size && tok->arr.arr[i+1][0] == '>')
		{
			ft_add(&new_one, "[APPPEND]");
			ft_add(&new_second, ">>");
			i++;
		}
		else if(tok->arr.arr[i][0] == '<')
		{
			ft_add(&new_one, "[INPUT]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		else if(tok->arr.arr[i][0] == '>')
		{
			ft_add(&new_one, "[OUTPUT]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		else
		{
			ft_add(&new_one, "[COMMAND]");
			ft_add(&new_second, tok->arr.arr[i]);
		}
		i++;
	}
	tok->category = new_one;
	tok->real = new_second;
}