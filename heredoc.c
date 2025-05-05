#include "minishell.h"

int ft_strlenn(char *s)
{
	int i;

	i = 0;
	while(s[i])
		i++;
	return (i);
}

int ft_strcmp(char *s, char *str)
{
	int	i;
	
	i = 0;
	while(s[i] && str[i] && str[i] == s[i])
		i++;
	return (str[i] - s[i]);
}
void ft_heredoc(t_tokens* tok, char *s, int k)
{
	int	i;
	char *str;

	//dir random smiya
	printf("\'%s\'\n",s);
	int fd = open("/tmp/dodo.txt", O_CREAT | O_WRONLY, 0777);
	int fd1 = open("/tmp/dodo.txt", O_CREAT | O_RDONLY, 0777);
	unlink("/tmp/dodo.txt");
	while(1)
	{
		str = readline(">");
		if(!str || ft_strcmp(str,s) == 0)
		{
			free(str);
			close(fd);
			break;
		}
		if (k == 1)
			str = ft_transform(tok, str);
		write(fd, str, ft_strlenn(str));
		write(fd, "\n", 1);
		free(str);
	}
	close(fd);
	//store(fd1);
}