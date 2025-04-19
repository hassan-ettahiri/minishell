/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 06:19:11 by hettahir          #+#    #+#             */
/*   Updated: 2025/04/19 00:53:23 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_num_len(long long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n *= -1;
	}
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	ft_romp(char **s, long long nb, int len)
{
	(*s)[len--] = 0;
	if (nb == 0)
	{
		(*s)[0] = '0';
		return ;
	}
	if (nb < 0)
	{
		(*s)[0] = '-';
		nb *= -1;
	}
	while (nb)
	{
		(*s)[len] = (nb % 10) + '0';
		nb /= 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	int			len;
	char		*s;
	long long	nb;

	nb = (long long)n;
	len = ft_num_len(nb);
	s = (char *)ft_malloc((len + 1) * sizeof(char));
	if (!s)
		return (NULL);
	ft_romp(&s, nb, len);
	return (s);
}
