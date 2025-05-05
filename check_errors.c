#include "minishell.h"
int ft_check(t_tokens *tok, int j)
{
	size_t i;
	int jump;
	int k;

	i = 0;
	while (i < tok->TWOD_categories[j].size)
	{
		jump = 0;
		if (tok->TWOD_categories[j].arr[i][1] == 'H')
		{
			if(i+1 < tok->TWOD_categories[j].size && tok->TWOD_categories[j].arr[i+1][1] == 'C')
			{
				k = 0;
				if(tok->TWOD_new_quoted[j].arr[i+1][0] == '1')
					k = 1;
				ft_heredoc(tok, tok->TWOD_joined[j].arr[i+1], k);
				jump++;
			}
			else if(i+2 < tok->TWOD_categories[j].size && tok->TWOD_categories[j].arr[i+1][1] == 'S' && tok->TWOD_categories[j].arr[i+2][1] == 'C')
			{
				k = 0;
				if(tok->TWOD_new_quoted[j].arr[i+2][0] == '1')
					k = 1;
				ft_heredoc(tok, tok->TWOD_joined[j].arr[i+2], k);
				jump += 2;
			}
			else
			{
				tok->wrong = 1;
				return (1);
			}
		}
		else if(tok->TWOD_categories[j].arr[i][1] == 'A' || tok->TWOD_categories[j].arr[i][1] == 'I' || tok->TWOD_categories[j].arr[i][1] == 'O')
		{
			if(i+1 < tok->TWOD_categories[j].size && tok->TWOD_categories[j].arr[i+1][1] == 'C')
				jump++;
			else if(i+2 < tok->TWOD_categories[j].size && tok->TWOD_categories[j].arr[i+1][1] == 'S' && tok->TWOD_categories[j].arr[i+2][1] == 'C')
				jump += 2;
			else
			{
				tok->wrong = 1;
				return (1);
			}
		}
		i += (jump + 1);
	}
	return (0);
}

void ft_check_errors(t_tokens *tok)
{
	int	i;
	i = 0;
	while(i < tok->length)
	{
		if((tok->TWOD_categories[i].size == 0 || (tok->TWOD_categories[i].size == 1 && tok->TWOD_categories[i].arr[0][1] == 'S')) && tok->length == 1)
		{
			return ;
		}
		if(tok->TWOD_categories[i].size == 0 || (tok->TWOD_categories[i].size == 1 && tok->TWOD_categories[i].arr[0][1] == 'S'))
		{
			//checki ila derti enter khawya
			tok->wrong = 1;
			break;
		}
		if(ft_check(tok, i))
				break;
		if(tok->wrong)
			break;
		i++;
	}
}