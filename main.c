#include "minishell.h"

int main(void)
{
	char		*s;
	t_tokens	tok;
	int			i;
	
	while(1)
	{
		s = readline("minishell$ ");
		if(!s)
			break;
		tok = ft_tokenization(s);
		if(tok.wrong)
		{
			printf("ERROR\n");
			continue;
		}
		ft_enumization(&tok);
		if (tok.wrong)
		{
			printf("ERROR\n");
			continue;
		}
		ft_piping(&tok);
		
		
		tok.e = NULL;
		ft_expanding(&tok);
		ft_joining(&tok);
		
		ft_check_errors(&tok);
		if (tok.wrong)
		{
			printf("ERROR\n");
			continue;
		}
		i = 0;
		while(i < tok.length)
		{
			int j = 0;
			
			j = 0;
			while(j < tok.TWOD_joined[i].size)
			{
				printf("%s",tok.TWOD_joined[i].arr[j]);
				j++;
			}
			if(tok.TWOD_categories[i].size != 0)
			printf("\n");
			j = 0;
			while(j < tok.TWOD_categories[i].size)
			{
				printf("%s",tok.TWOD_categories[i].arr[j]);
				j++;
			}
			int l = 0;
			if(tok.TWOD_categories[i].size != 0)
			printf("\n");
			i++;
		}
		add_history(s);
		//printf("%s\n",s);
	}
	return (0);
}
//handle command khawiya
//expanding
//expandi heredoc inside
//cat << $"$'$"$USER | d | DD | dsdf | fdsf | dfs| dfsf|fdsf| fdsf| fdf
//handli $ bou7do
//"" not found , $asd return nothing