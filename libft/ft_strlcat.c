/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:00:33 by hettahir          #+#    #+#             */
/*   Updated: 2025/04/19 00:53:23 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	int		i;
	int		j;
	size_t	d;

	if (dstsize == 0)
		return (ft_strlen(src));
	i = 0;
	while (dest[i])
		i++;
	j = 0;
	d = i;
	if (d >= dstsize)
		return (dstsize + ft_strlen(src));
	while (src[j] && dstsize - i - 1 > 0)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = 0;
	return (d + ft_strlen(src));
}
