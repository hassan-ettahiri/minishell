#include "../minishell.h"

static int	check_len(const char *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
    if(s[0] == '+' || s[0] == '-')
        i++;
	while (s[i] == '0')
		i++;
	while (ft_isdigit(s[i + count]))
		count++;
	return (count > ft_strlen("9223372036854775807")
		|| (count == ft_strlen("9223372036854775807")
			&& ft_strncmp(&s[i], "9223372036854775807", count) > 0));
}

int checklen_and_is_alpha(char *str)
{
    int i = 0;

    if(str[0] == '+' || str[0] == '-')
        i++;

    while(str[i])
    {
        if(!(str[i] >= '0' && str[i] <= '9'))
            return 1;
        i++;
    }
    return 0;
}

int exit_status(char **params, int nb_param, int status)
{
    char a;
    if (nb_param > 1)
    {
        write(1, " exit\n", 6);
        write(1, "exit: too many arguments\n", 24);
        return 1;
    }
    if (nb_param == 1)
    {
        if(checklen_and_is_alpha(params[0]) == 1 || check_len(params[0]) == 1){
            printf("exit\nexit: %s: numeric argument required\n", params[0]);
            exit(2);
        }
        a = ft_atoi(params[0]);
        ft_malloc(-1);
        exit(a);
    }
    ft_malloc(-1);
    exit(status);
}
