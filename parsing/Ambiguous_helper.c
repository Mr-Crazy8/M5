/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ambiguous_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 17:22:57 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 17:23:46 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	check_for_space_helper(int *in_word, int *word_count)
{
	(*word_count)++;
	(*in_word) = 1;
}

int	check_for_space(char *str)
{
	int	i;
	int	word_count;
	int	in_word;

	i = 0;
	word_count = 0;
	in_word = 0;
	if (!str)
		return (0);
	while (str && str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
		{
			if (!in_word)
				check_for_space_helper(&in_word, &word_count);
		}
		else
			in_word = 0;
		i++;
	}
	if (word_count > 1)
		return (1);
	return (0);
}

char	*trim_space(char *str)
{
	int		i;
	int		start;
	int		end;
	char	*trim_str;

	i = 0;
	while (str && str[i] && str[i] == ' ')
		i++;
	start = i;
	while (str && str[i] && str[i] != ' ')
		i++;
	end = i;
	trim_str = ft_substr(str, start, end - start);
	return (trim_str);
}