/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntx_check_helper_0.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:26:07 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/31 10:51:21 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_for_redirection(char *str)
{
	int	i;
	int	count;
	int len;

	i = 0;
	len = 0;
	count = 0;
	if (str != NULL && strcmp(str, "|") != 0)
	{
		len = ft_strlen(str);
		while (i <= len && str[i])
		{
			redirection_helper(str, &i, &count);
			if (str[i] == '>' || str[i] == '<')
			{
				count++;
				i++;
			}
			else
				i++;
		}
	}
	return (count);
}

void	check_missing_filename(char *str)
{
	int		i;
	char	operator_type;

	i = 0;
	if (str != NULL && check_for_redirection(str) != 0)
	{
		while (str[i] && str[i] != '>' && str[i] != '<')
			i++;
		if (str[i] == '>' || str[i] == '<')
		{
			operator_type = str[i];
			i++;
			if (str[i] == operator_type)
				i++;
			while (str[i] && str[i] == ' ')
				i++;
			if (str[i] == '\0' || str[i] == '>' || str[i] == '<')
				write(2,
					"minishell:  syntax error \
						near unexpected token `newline'\n",
					58);
		}
	}
}

void	print_error(char *str, int *i)
{
	if (str[(*i)] == '\0')
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
	else if (str[(*i)] == '|')
		write(2, "minishell: syntax error near unexpected token `|'\n",
			51);
	else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
		write(2, "minishell: syntax error near unexpected token `>>'\n",
			52);
	else if (str[(*i)] == '>')
		write(2, "minishell: syntax error near unexpected token `>'\n",
			51);
	else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
		write(2, "minishell: syntax error near unexpected token `<<'\n",
			52);
	else if (str[(*i)] == '<')
		write(2, "minishell: syntax error near unexpected token `<'\n",
			51);
}

// int	invalid_filename_checker(char *str, int *i)
// {
// 	char	operator_type;

// 	operator_type = str[(*i)];
// 	(*i)++;
// 	if (str[(*i)] == operator_type)
// 		(*i)++;
// 	while (str[(*i)] && str[(*i)] == ' ')
// 		(*i)++;
// 	if (str[(*i)] == '\0' || str[(*i)] == '>'
// 		|| str[(*i)] == '<' || str[(*i)] == '|')
// 	{
// 		print_error(str, i);
// 		return (1);
// 	}
// 	else
// 	{
// 		while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != '>'
// 			&& str[(*i)] != '<' && str[(*i)] != '|')
// 			(*i)++;
// 	}
// 	return (0);
// }

int	invalid_filename_checker(char *str, int *i)
{
	char	operator_type;
	int     quote_state;  // 0: no quote, 1: single quote, 2: double quote

	quote_state = 0;
	operator_type = str[(*i)];
	(*i)++;
	if (str[(*i)] == operator_type)
		(*i)++;
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
	if (str[(*i)] == '\0' || 
	   (str[(*i)] == '>' && quote_state == 0) || 
	   (str[(*i)] == '<' && quote_state == 0) || 
	   (str[(*i)] == '|' && quote_state == 0))
	{
		print_error(str, i);
		return (1);
	}
	else
	{
		// Process filename with quote handling
		while (str[(*i)])
		{
			// Update quote state
			if (str[(*i)] == '\'')
			{
				if (quote_state == 0)
					quote_state = 1;
				else if (quote_state == 1)
					quote_state = 0;
			}
			else if (str[(*i)] == '"')
			{
				if (quote_state == 0)
					quote_state = 2;
				else if (quote_state == 2)
					quote_state = 0;
			}
			
			if (quote_state == 0 && (str[(*i)] == ' ' || 
				str[(*i)] == '>' || str[(*i)] == '<' || str[(*i)] == '|'))
				break;
			
			(*i)++;
		}
	}
	return (0);
}


int	check_invalid_filename(char *str)
{
	int	i;
	int	found_error;

	i = 0;
	found_error = 0;
	if (str != NULL && check_for_redirection(str) != 0)
	{
		while (str[i] && !found_error)
		{
			while (str[i] && str[i] != '>' && str[i] != '<')
				i++;
			if (!str[i])
				break ;
			if (str[i] == '>' || str[i] == '<')
				found_error = invalid_filename_checker(str, &i);
		}
	}
	return (found_error);
}
