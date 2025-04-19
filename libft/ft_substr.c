/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:29:04 by hettahir          #+#    #+#             */
/*   Updated: 2025/04/19 00:53:23 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_alloc(char **ptr, char const *s, unsigned int start, size_t len)
{
	if (ft_strlen(s) <= start)
	{
		*ptr = (char *)ft_malloc(sizeof(char));
		if (!*ptr)
			return (0);
		return (2);
	}
	if (ft_strlen(s) < len + start)
		*ptr = (char *)ft_malloc((ft_strlen(s) - start + 1) * sizeof(char));
	else
		*ptr = (char *)ft_malloc((len + 1) * sizeof(char));
	if (!*ptr)
		return (0);
	return (1);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;
	size_t	j;
	int		c;

	if (!s)
		return (NULL);
	c = ft_alloc(&ptr, s, start, len);
	if (c == 2)
	{
		ptr[0] = '\0';
		return (ptr);
	}
	if (c == 0)
		return (NULL);
	i = start;
	j = 0;
	while (i < start + len && s[start + j])
	{
		ptr[j] = s[i];
		i++;
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}
