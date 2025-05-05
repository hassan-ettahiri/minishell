#include "minishell.h"

void ft_piping(t_tokens *tok)
{
	int	i;
	int	k;
	STArray *p;
	i = 0;
	int l = 0;
	while(i < tok->category.size)
	{
		if(tok->category.arr[i][1] == 'P')
			l++;
		i++;
	}
	tok->length = l + 1;
	tok->TWOD_array_cat = malloc(sizeof(STArray) * (tok->length));
	tok->TWOD_array_real = malloc(sizeof(STArray) * (tok->length));
	tok->TWOD_array_exp = malloc(sizeof(STArray) * (tok->length));
	tok->TWOD_real_deal = malloc(sizeof(STArray) * (tok->length));
	tok->TWOD_quoted = malloc(sizeof(STArray) * (tok->length));
	i = 0;
	while(i < tok->length)
	{
		ft_init(&tok->TWOD_array_cat[i]);
		ft_init(&tok->TWOD_array_real[i]);
		ft_init(&tok->TWOD_array_exp[i]);
		ft_init(&tok->TWOD_real_deal[i]);
		ft_init(&tok->TWOD_quoted[i]);
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
		if(tok->category.arr[i][1] == 'C' && (tok->real.arr[i][0] == '\"' || tok->real.arr[i][0] == '\''))
			ft_add(&tok->TWOD_quoted[k] ,ft_substrr("1",0,0));
		else
			ft_add(&tok->TWOD_quoted[k] ,ft_substrr("0",0,0));
		if (tok->category.arr[i][1] == 'H')
			 l = 1;
		else if(tok->category.arr[i][1] != 'C' && tok->category.arr[i][1] != 'S')
			l = 0;
		ft_add(&tok->TWOD_array_cat[k] ,tok->category.arr[i]);
		ft_add(&tok->TWOD_array_real[k] ,tok->real.arr[i]);
		if(tok->category.arr[i][1] == 'C' && tok->real.arr[i][0] == '\'')
			ft_add(&tok->TWOD_array_exp[k] ,ft_substrr("0",0,0));
		else if(tok->category.arr[i][1] == 'C' && l == 1)
			ft_add(&tok->TWOD_array_exp[k] ,ft_substrr("0",0,0));
		else if(tok->category.arr[i][1] == 'C' && tok->real.arr[i][0] == '\"')
			ft_add(&tok->TWOD_array_exp[k] ,ft_substrr("2",0,0));
		else if(tok->category.arr[i][1] == 'C')
			ft_add(&tok->TWOD_array_exp[k] ,ft_substrr("1",0,0));
		else
			{
				ft_add(&tok->TWOD_array_exp[k] ,ft_substrr("0",0,0));
			}
		if(tok->category.arr[i][1] == 'C')
		{
			if(tok->real.arr[i][0] == '\'')
				ft_add(&tok->TWOD_real_deal[k] ,ft_substrr(tok->real.arr[i],1,ft_strlenn(tok->real.arr[i])-2));
			else if(tok->real.arr[i][0] == '\"')
				ft_add(&tok->TWOD_real_deal[k] ,ft_substrr(tok->real.arr[i],1,ft_strlenn(tok->real.arr[i])-2));
			else
				ft_add(&tok->TWOD_real_deal[k] ,tok->real.arr[i]);
		}
		else
			ft_add(&tok->TWOD_real_deal[k] ,tok->real.arr[i]);
		i++;
	}
}
//unquoting '' nothing