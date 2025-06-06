/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:48:37 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/05 22:49:39 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char *trim_whitespace(char *str)
{

    // int i = 0;
    int start = 0;
    int lent ;

     if (!str)
        return NULL;

    while (str[start] && is_whitespace(str[start]))
        start++;
    
    lent = ft_strlen(&str[start]);
    
    return(ft_substr(str, start, lent));
}

// int pls_conter_helper(char *str, int *pls_count, int *j, int *i)
// {

//     if (str[(*i)] == '+' && str[(*i) + 1] == '=' && ((*pls_count) == 0 || (*pls_count) == 1) )
//     {
//         if ((*i) == 0 || (!isalpha(str[0]) && str[0] != '_'))
//             return 0; // Invalid key name
//         while ((*j) < (*i))
//         {
//             if (!isalnum(str[(*j)]) && str[(*j)] != '_')
//                 return 0; // Invalid character in key name
//             (*j)++;
//         }
//         return 1; // Valid += assignment
//     }
//     return 0;

// }

// int pls_conter_helper(char *str, int *pls_count, int *j, int *i)
// {
//     // Remove the strchr checks for quotes
//     if (str[(*i)] == '+' && str[(*i) + 1] == '=' && ((*pls_count) == 0 || (*pls_count) == 1))
//     {
//         if ((*i) == 0 || (!isalpha(str[0]) && str[0] != '_'))
//             return 0; // Invalid key name
//         while ((*j) < (*i))
//         {
//             // Consider quoted characters differently
//             if (str[(*j)] == '\'' || str[(*j)] == '"')
//             {
//                 (*j)++;
//                 continue;
//             }
            
//             // Normal validation for non-quoted characters
//             if (!isalnum(str[(*j)]) && str[(*j)] != '_')
//                 return 0; // Invalid character in key name
            
//             (*j)++;
//         }
//         return 1; // Valid += assignment
//     }
//     return 0;
// }
int pls_conter_helper(char *str, int *pls_count, int *j, int *i)
{
    // Remove the quote checks completely
    if (str && str[(*i)] == '+' && str[(*i) + 1] == '=' && ((*pls_count) == 0 || (*pls_count) == 1))
    {
        // Check only the part before '+='
        int k = 0;
        while (k < (*i))
        {
            if ((k == 0 && !isalpha(str[k]) && str[k] != '_') || 
                (k > 0 && !isalnum(str[k]) && str[k] != '_' && str[k] != '\'' && str[k] != '"'))
                return 0;  // Invalid character in variable name
            k++;
        }
        return 1;  // Valid append operation
    }
    return 0;
}
int pls_conter(char *str)
{
    int i;
    int pls_count;
    int j;
    
    i = 0;
    pls_count = 0;
    j = 1;
    while (str && str[i])
    {
        if (str[i] == '+')
            pls_count++;
        i++;
    }
    i = 0;
    while (str && str[i] && str[i] != '+')
        i++;
    return(pls_conter_helper(str, &pls_count, &j, &i));
}

int helper3(t_exp_helper *expand, int exit_status, int pipe_out)
{
    static int was_there_a_pipe_befor = 0;

    if (pipe_out == 1)
        was_there_a_pipe_befor++;
	if (expand->original[expand->i] == '?')
	{
        if (was_there_a_pipe_befor == 1)
        {
            was_there_a_pipe_befor = 0;
            expand->var_value = ft_itoa(0);
        }
        else
		    expand->var_value = ft_itoa(get_or_set(GET, 0));
		expand->i++;
		return (1);
	}
	return (0);
}

int expand_handle_helper0(t_exp_helper *expand)
{
	if (expand->original[expand->i] == '\'')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 1;
		else if (expand->quote_state == 1)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	else if (expand->original[expand->i] == '"')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 2;
		else if (expand->quote_state == 2)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	return (0);
}
