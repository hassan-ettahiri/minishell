#include "minishell.h"

int ft_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}
void	ft_clear(STArray *dd)
{
	free(dd->arr);
}

char	**ft_realloc(STArray *dd, size_t size , size_t cap)
{
	char	**new_one;
	int		i;

	new_one = (char **)malloc(sizeof(char *) * cap);
	i = 0;
	while(i < size)
	{
		new_one[i] = dd->arr[i];
		i++;
	}
	ft_clear(dd);
	return (new_one);
}

char	*ft_substrr(char *s, int st, int ed)
{
	int		i;
	char	*final;

	final = malloc(sizeof(char) * (ed-st+2));
	if(!final)
		return (NULL);
	i = 0;
	while(i+st <= ed)
	{
		final[i] = s[st+i];
		i++;
	}
	final[i] = '\0';
	return (final);
}

void ft_init(STArray *dd)
{
	dd->size = 0;
	dd->capacity = 4;
	dd->arr = (char **)malloc(sizeof(char *) * dd->capacity);
}

void	ft_add(STArray *dd, char *s)
{
	if(dd->size == dd->capacity)
	{
		dd->capacity *= 2ll;
		dd->arr = ft_realloc(dd, dd->size, dd->capacity);
	}
	dd->arr[dd->size] = s;
	dd->size++;
}
void ft_splittt_strings(STArray *arr, char *s)
{
	int	i;
	int	k;

	i = 0;
	k = -1;
	while(s[i])
	{
		if(s[i] == '<' || s[i] == '>' || s[i] == '|')
		{
			if(k != -1)
				ft_add(arr, ft_substrr(s, k, i-1));
			k = -1;
			ft_add(arr, ft_substrr(s, i, i));
		}
		else if(k == -1)
				k = i;
		i++;
	}
	if(k != -1)
		ft_add(arr, ft_substrr(s, k , i - 1));
}
void ft_tokenization_secondo(t_tokens *tok)
{
	int	i;

	i = 0;
	STArray new_one;
	ft_init(&new_one);
	while(i < tok->arr.size)
	{
		if(tok->arr.arr[i][0] == '\"')
			ft_add(&new_one, tok->arr.arr[i]);
		else if(tok->arr.arr[i][0] == '\'')
			ft_add(&new_one, tok->arr.arr[i]);
		else if(ft_space(tok->arr.arr[i][0]))
			ft_add(&new_one, tok->arr.arr[i]);
		else
			ft_splittt_strings(&new_one,tok->arr.arr[i]);
		i++;
	}
	tok->arr = new_one;
}
t_tokens ft_tokenization(char *s)
{
	int			i;
	t_tokens	tok;
	int			double_quote;
	int			single_quote;
	int			space_op;
	int 		k;

	i = 0;
	tok.wrong = 0;
	ft_init(&tok.arr);
	double_quote = 0;
	single_quote = 0;
	space_op = 0;
	k = 0;
	while(s[i])
	{
		if(double_quote)
		{
			if(s[i] == '\"')
			{
				double_quote = 0;
				ft_add(&tok.arr, ft_substrr(s, k,i));
				k = i + 1;
			}
			i++;
			continue;
		}
		if(single_quote)
		{
			if(s[i] == '\'')
			{
				single_quote = 0;
				ft_add(&tok.arr, ft_substrr(s, k,i));
				k = i + 1;
			}
			i++;
			continue;
		}
		if(s[i] == '\'')
		{
			single_quote = 1;
			if(k != i)
				ft_add(&tok.arr, ft_substrr(s, k,i-1));
			k = i;
			i++;
			continue;
		}
		if(s[i] == '\"')
		{
			double_quote = 1;
			if(k != i)
				ft_add(&tok.arr, ft_substrr(s, k,i-1));
			k = i;
			i++;
			continue;
		}
		if(space_op)
		{
			if(!ft_space(s[i]))
			{
				space_op = 0;
				ft_add(&tok.arr, ft_substrr(s, k, i - 1));
				k = i;
				continue;
			}
			i++;
			continue;
		}
		if(ft_space(s[i]))
		{
			space_op = 1;
			if(k != i)
			ft_add(&tok.arr, ft_substrr(s, k, i - 1));
			k = i;
			i++;
			continue;
		}
		i++;
	}
	if(double_quote || single_quote)
		tok.wrong = 1;
	if(k != i)
		ft_add(&tok.arr, ft_substrr(s, k, i-1));
	ft_tokenization_secondo(&tok);
	return (tok);
}