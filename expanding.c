#include "minishell.h"

char *ft_strjoinn(char *s1, char *s2)
{
	int		i;
	int		k;
	char	*res;
	int		j;

	k = ft_strlenn(s1) + ft_strlenn(s2) + 1;
	res = malloc(k * sizeof(char));
	i = 0;
	while(s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		res[i] = s2[j];
		j++;
		i++;
	}
	res[i] = '\0';
	return (res);
}
char	*ft_search(t_tokens *tok, char *s)
{
	t_env *p;

	p = tok->e;
	char *res = NULL;
	res = ft_substrr("",0,0);
	//printf("%s\n",s);
	while(p)
	{
		if(ft_strcmp(s, p->key) == 0)
		{
			res = ft_substrr(p->value, 0, ft_strlenn(p->value)-1);
			break;
		}
		p = p->next;
	}
	return (res);
}
char *ft_transform(t_tokens *tok, char *s)
{
	int		i;
	char	*res;
	int		j;
	i = 0;
	j = 0;
	res = malloc(1);
	res[0] = '\0';
	while(s[i])
	{
		if(s[i] != '$')
		{
			j = i;
			while(s[j] && s[j] != '$')
				j++;
			res = ft_strjoinn(res, ft_substrr(s,i, j - 1));
			i = j;
		}
		else
		{
			j = i;
			j++;
			if(!((s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= 'a' && s[j] <= 'z') || s[j] == '_'))
			{
				i = j;
				continue;
			}
			while(s[j] && ((s[j] >= 'A' && s[j] <= 'Z') || ((s[j] >= 'a' && s[j] <= 'z')) || s[j] == '_' || (s[j] >= '0' && s[j] <= '9')))
				j++;
			//check later i+1
			res = ft_strjoinn(res, ft_search(tok ,ft_substrr(s,i+1, j - 1)));
			i = j;
		}
	}
	return (res);
}

void ft_expanding(t_tokens *tok)
{
	size_t	i;
	int	k;
	i = 0;
	int l = 0;
	while(i < tok->category.size)
	{
		if(tok->category.arr[i][1] == 'P')
			l++;
		i++;
	}
	tok->length = l + 1;
	tok->TWOD_expanded = malloc(sizeof(STArray) * (tok->length));
	i = 0;
	while(i < (size_t)tok->length)
	{
		ft_init(&tok->TWOD_expanded[i]);
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
		if(tok->category.arr[i][1] == 'C' && tok->TWOD_array_exp[k].arr[l][0] == '2'){
			ft_add(&tok->TWOD_expanded[k] ,ft_transform(tok, tok->TWOD_real_deal[k].arr[l]));
		}
		else if(tok->category.arr[i][1] == 'C' && tok->TWOD_array_exp[k].arr[l][0] == '1'){
			ft_add(&tok->TWOD_expanded[k] ,ft_transform(tok, tok->TWOD_real_deal[k].arr[l]));
		}
		else
			ft_add(&tok->TWOD_expanded[k] ,tok->TWOD_real_deal[k].arr[l]);
		i++;
		l++;
	}
}
