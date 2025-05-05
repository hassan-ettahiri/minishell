/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:09:04 by hettahir          #+#    #+#             */
/*   Updated: 2025/05/05 10:10:56 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_len(const char *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i] == '0')
		i++;
	while (ft_isdigit(s[i + count]))
		count++;
	return (count > ft_strlen("9223372036854775807")
		|| (count == ft_strlen("9223372036854775807")
			&& ft_strncmp(&s[i], "9223372036854775807", count) > 0));
}

static int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v'
		|| c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(const char *s)
{
	long	num;
	int		sign;
	size_t	i;

	i = 0;
	num = 0;
	sign = 1;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (s[i] == '+')
		i++;
	if (check_len(&s[i]))
	{
		if (sign == 1)
			return (-1);
		return (0);
	}
	while (ft_isdigit(s[i]))
		num = (num * 10) + (s[i++] - 48);
	return (num * sign);
}
