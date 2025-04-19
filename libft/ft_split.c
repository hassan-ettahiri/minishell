/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 03:48:05 by hettahir          #+#    #+#             */
/*   Updated: 2025/04/19 00:53:23 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_cont_word(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
			count++;
		s++;
	}
	return (count);
}

static int	ft_word_len(char const *s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i])
	{
		if (s[i] == c)
			break ;
		i++;
		len++;
	}
	return (len);
}

static char	*ft_copy_word(char const *s, char c, int *i)
{
	int		len;
	int		j;
	char	*str;

	j = 0;
	while (s[*i])
	{
		if (s[*i] == c)
			(*i)++;
		else
			break ;
	}
	len = ft_word_len(s, c, *i);
	str = (char *)ft_malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (j < len)
	{
		str[j] = s[*i];
		j++;
		(*i)++;
	}
	str[j] = 0;
	return (str);
}


char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		words;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	j = 0;
	i = 0;
	words = ft_cont_word(s, c);
	strs = (char **)ft_malloc((words + 1) * sizeof(char *));
	if (!strs)
		return (NULL);
	while (j < words)
	{
		strs[j] = ft_copy_word(s, c, &i);
		if (!strs[j])
		{
			return (NULL);
		}
		j++;
	}
	strs[j] = 0;
	return (strs);
}
