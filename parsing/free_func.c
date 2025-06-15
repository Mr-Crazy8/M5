/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:38 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 16:55:23 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	**free_split(char **split, size_t j)
{
	while (j > 0)
		free(split[--j]);
	free(split);
	return (NULL);
}

void	free_env_struct(t_env *env_struct)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env_struct;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}
}

void	free_token_list(t_token *token_list)
{
	t_token	*current;
	t_token	*next;

	current = token_list;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current->tokin);
		free(current);
		current = next;
	}
}

char	**free_split_q(char **split, size_t j)
{
	while (j > 0)
		free(split[--j]);
	free(split);
	return (NULL);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_expanded(t_exp_helper *expand)
{
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
