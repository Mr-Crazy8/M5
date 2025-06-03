/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:51:19 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/01 11:56:24 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int ensure_buffer_space_hp(t_exp_helper *expand)
{
    if (!expand->buffer_size)
	{
		size_t original_len = strlen(expand->original);
		if (original_len > SIZE_MAX / 2 - 1024)
		{
            write(2, "minishell: memory allocation failed: input too large\n" , 54);
			return (0);
		}
		expand->buffer_size = original_len * 2 + 1024;
	}
    return 1;
}
int ensure_buffer_space(t_exp_helper *expand, size_t additional_needed)
{
    char *new_buffer;
    size_t new_size;

    if (!ensure_buffer_space_hp(expand))
        return (0);
	if (expand->j + additional_needed >= expand->buffer_size)
	{
		new_size = expand->buffer_size * 2;
		if (new_size < expand->j + additional_needed + 1)
			new_size = expand->j + additional_needed + 1024;
		new_buffer = ft_calloc(1, new_size);
		if (!new_buffer)
		{
            write(2, "minishell: memory allocation failed\n", 37);
			exit(1);
        }
		if (expand->expanded)
		{
			memcpy(new_buffer, expand->expanded, expand->j);
			free(expand->expanded);
		}
		expand->expanded = new_buffer;
		expand->buffer_size = new_size;
	}
	return (1);
}

