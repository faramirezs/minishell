/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alramire <alramire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:27:01 by alramire          #+#    #+#             */
/*   Updated: 2024/05/08 08:52:06 by alramire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **strs, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static int	ft_words(char const *s, char d)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if ((s[i] != d && (s[i - 1] == d || i == 0)))
			j++;
		i++;
	}
	return (j);
}

int	ft_words_chars(char const *s, char d, int word)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (s[i])
	{
		if (s[i] != d && (i == 0 || s[i - 1] == d))
			word--;
		if (word == 0)
		{
			while (s[i] != d && s[i] != '\0')
			{
				k++;
				i++;
			}
			break ;
		}
		while (s[i] != d && s[i] != '\0')
			i++;
		if (s[i] == d)
			i++;
	}
	return (k);
}

char	**ft_writer(char const *s, char **tmp, char c, int words)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (i < words)
	{
		while (s[j] == c)
			j++;
		k = 0;
		while (s[j] != c && s[j] != '\0')
		{
			tmp[i][k] = s[j];
			k++;
			j++;
		}
		tmp[i][k] = '\0';
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**tmp;
	int		i;

	if (!s)
		return (NULL);
	words = ft_words(s, c);
	tmp = (char **)malloc(sizeof(char *) * (words + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < words)
	{
		tmp[i] = (char *)malloc(ft_words_chars(s, c, i + 1) + 1);
		if (!tmp[i])
		{
			ft_free(tmp, i);
			return (NULL);
		}
		i++;
	}
	ft_writer(s, tmp, c, words);
	return (tmp);
}
