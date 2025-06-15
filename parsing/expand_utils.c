/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:16:14 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 12:22:02 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	**split_if_needed(char *str)
{
	int		i;
	int		count;
	char	**result;
	char	*trimmed_str;

	i = 0;
	if (!str || !*str)
		return (NULL);
	trimmed_str = ft_strtrim(str, " ");
	if (!trimmed_str)
		return (NULL);
	if (!strchr(trimmed_str, ' ') && !strchr(trimmed_str, '\t')
		&& !strchr(trimmed_str, '\n'))
		return (free(trimmed_str), NULL);
	result = ft_split_q(trimmed_str, ' ');
	free(trimmed_str);
	count = 0;
	if (result)
	{
		while (result[count])
			count++;
	}
	if (count <= 1)
		return (free_string_array(result), NULL);
	return (result);
}

int	ft_lint(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void	prepare_new_args(char **new_args, t_cmd *current, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		new_args[j] = current->args[j];
		j++;
	}
	free(current->args[i]);
}

int	rebuild_cmd_args(char **new_args, t_cmd *current, char **split,
					int i, int word_count)
{
	int	j;
	int	arg_count;

	j = 1;
	arg_count = ft_lint(current->args);
	new_args[i] = strdup(split[0]);
	while (j < word_count)
	{
		new_args[i + j] = strdup(split[j]);
		j++;
	}
	j = i + 1;
	while (j < arg_count)
	{
		new_args[j + word_count - 1] = current->args[j];
		j++;
	}
	new_args[arg_count + word_count - 1] = NULL;
	free(current->args);
	current->args = new_args;
	return (i + word_count - 1);
}

void	split_the_rest_helper(t_cmd *current, int *i)
{
	char	**split;
	int		word_count;
	char	**new_args;

	split = split_if_needed(current->args[(*i)]);
	if (split && split[1])
	{
		word_count = ft_lint(split);
		new_args = malloc(sizeof(char *)
				* (ft_lint(current->args) + word_count));
		if (new_args)
		{
			prepare_new_args(new_args, current, (*i));
			(*i) = rebuild_cmd_args(new_args, current, split,
					(*i), word_count);
		}
	}
	if (split)
		free_string_array(split);
}

int	split_the_rest_hp(t_cmd *current, int *i)
{
	char	*equals;

	if (!current->args)
		return (0);
	if (!current->cmd || strcmp(current->cmd, "export") != 0)
	{
		(*i) = 0;
		while (current->args && current->args[(*i)])
		{
			equals = strchr(current->args[(*i)], '=');
			split_the_rest_helper(current, i);
			(*i)++;
		}
		return (1);
	}
	return (0);
}

void	split_the_rest(t_cmd *current)
{
	int	i;

	if (split_the_rest_hp(current, &i))
		return ;
	i = 1;
	while (current->args && current->args[i])
	{
		split_the_rest_helper(current, &i);
		i++;
	}
}

void	apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		split_the_rest(current);
		current = current->next;
	}
	if (expand)
	{
		if (expand->expanded)
			free(expand->expanded);
		if (expand->var_name)
			free(expand->var_name);
		if (expand->var_value)
			free(expand->var_value);
		free(expand);
	}
}
