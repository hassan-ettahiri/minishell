#include "minishell.h"
char *ft_join(char *s0, char *s1)
{
	char *res;
	int		i;
	int		j;

	res = malloc(ft_strlenn(s0) + ft_strlenn(s1) + 1);
	i = 0;
	while(s0[i])
	{
		res[i] = s0[i];
		i++;
	}	
	j = 0;
	while(s1[j])
	{
		res[i+j] = s1[j];
		j++;
	}
	free(s0);
	res[j+i] = '\0';
	return (res);
}

void ft_joining(t_tokens *tok)
{
	int	i;
	int	k;
	STArray *p;
	char	*s;
	int 	j;
	i = 0;
	int l = 0;
	int gg = 0;
	tok->TWOD_joined = malloc(sizeof(STArray) * (tok->length));
	tok->TWOD_categories = malloc(sizeof(STArray) * (tok->length));
	tok->TWOD_new_quoted = malloc(sizeof(STArray) * (tok->length));
	i = 0;
	while(i < tok->length)
	{
		ft_init(&tok->TWOD_joined[i]);
		ft_init(&tok->TWOD_categories[i]);
		ft_init(&tok->TWOD_new_quoted[i]);
		i++;
	}
	i = 0;
	k = 0;
	l = 0;
	while(i < tok->category.size)
	{
		if(tok->category.arr[i][1] == 'P')
		{
			k++;
			i++;
			l = 0;
			continue;
		}
		if(tok->category.arr[i][1] == 'C')
		{
			s = ft_substrr("",0,-1);
			j = i;
			gg = 0;
			while(j < tok->category.size && tok->category.arr[j][1] == 'C')
			{
				s = ft_strjoinn(s, tok->TWOD_expanded[k].arr[l]);
				if(tok->TWOD_quoted[k].arr[l][0] >= '1')
					gg = 1;
				j++;
				l++;
			}
			l--;
			j--;
			i = j;
			if (gg == 1)
				ft_add(&tok->TWOD_new_quoted[k] ,ft_substrr("1",0,0));
			else
				ft_add(&tok->TWOD_new_quoted[k] ,ft_substrr("0",0,0));
			ft_add(&tok->TWOD_joined[k] ,s);
			ft_add(&tok->TWOD_categories[k] ,tok->category.arr[i]);
		}
		else
		{
			ft_add(&tok->TWOD_joined[k] ,tok->TWOD_expanded[k].arr[l]);
			ft_add(&tok->TWOD_categories[k] ,tok->category.arr[i]);
			if (tok->TWOD_quoted[k].arr[l][0] == '1')
				ft_add(&tok->TWOD_new_quoted[k] ,ft_substrr("1",0,0));
			else
				ft_add(&tok->TWOD_new_quoted[k] ,ft_substrr("0",0,0));
		}
		i++;
		l++;
	}
}
