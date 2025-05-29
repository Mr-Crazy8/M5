/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:00:49 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/27 18:08:29 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"


static	int	ft_count_strings(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			count++;
		}
		while (s[i] != '\0' && s[i] != c)
		{
			i++;
		}
	}
	return (count);
}

static int	ft_lent_strlen(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	return (i);
}

static char	*ft_word(char const *s, char c)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	len_word = ft_lent_strlen(s, c);
	word = (char *)malloc(len_word + 1);
	if (word == NULL)
	{
		return (NULL);
	}
	while (i < len_word)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	ft_fill_string(char **str, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			str[j] = ft_word(&s[i], c);
			if (str[j] == NULL)
				return (0);
			j++;
		}
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	str[j] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	int		i;

	i = 0;
	if (s == NULL)
		return (NULL);
	strings = (char **)malloc(sizeof(char *) * (ft_count_strings(s, c) + 1));
	if (strings == NULL)
	{
		return (NULL);
	}
	if (ft_fill_string(strings, s, c) == 0)
	{
		while (strings[i] != NULL)
		{
			free(strings[i]);
			i++;
		}
		free(strings);
		return (NULL);
	}
	return (strings);
}
