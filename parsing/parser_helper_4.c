/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:25:27 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 15:38:06 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h" 

int	creat_redir_list_helper(char *str)
{
	if (str && strcmp(str, ">") != 0
		&& strcmp(str, ">>") != 0
		&& strcmp(str, "<") != 0
		&& strcmp(str, "<<") != 0)
		return (1);
	return (0);
}

t_redir	*creat_redir_list_helper0(char *str1, char *str2)
{
	char	*str;

	if (!str2 || creat_redir_list_helper(str2) != 1)
		return (NULL);
	str = ft_strdup(str2);
	if (!str)
		return (NULL);
	if (strcmp(str1, ">>") == 0)
		return (creat_redir_node(2, str));
	else if (strcmp(str1, "<<") == 0)
		return (creat_redir_node(3, str));
	else if (strcmp(str1, ">") == 0)
		return (creat_redir_node(1, str));
	else if (strcmp(str1, "<") == 0)
		return (creat_redir_node(0, str));
	free(str);
	return (NULL);
}

t_redir	*creat_redir_list(char *str)
{
	t_redir	*tmp_node;
	t_redir	*tmp_list;
	char	**split_str;
	char	*redir;
	int		i;

	i = 0;
	tmp_list = NULL;
	tmp_node = NULL;
	redir = redir_extracter(str);
	split_str = ft_split_q(redir, ' ');
	if (!split_str)
		return (NULL);
	free(redir);
	while (split_str[i])
	{
		if ((strcmp(split_str[i], ">") == 0
				|| strcmp(split_str[i], ">>") == 0
				|| strcmp(split_str[i], "<") == 0
				|| strcmp(split_str[i], "<<") == 0)
			&& (!split_str[i + 1] || split_str[i + 1][0] == '\0'))
		{
			if (strcmp(split_str[i], ">") == 0)
				tmp_node = creat_redir_node(1, ft_strdup(""));
			else if (strcmp(split_str[i], ">>") == 0)
				tmp_node = creat_redir_node(2, ft_strdup(""));
			else if (strcmp(split_str[i], "<") == 0)
				tmp_node = creat_redir_node(0, ft_strdup(""));
			else if (strcmp(split_str[i], "<<") == 0)
				tmp_node = creat_redir_node(3, ft_strdup(""));
			if (tmp_node)
				add_redir_back(&tmp_list, tmp_node);
			i++;
		}
		else
		{
			tmp_node = NULL;
			tmp_node = creat_redir_list_helper0(split_str[i], split_str[i + 1]);
			if (tmp_node)
			{
				add_redir_back(&tmp_list, tmp_node);
				i++;
			}
			i++;
		}
	}
	free_split_str(split_str);
	return (tmp_list);
}
