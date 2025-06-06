/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:21:58 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/06 10:17:38 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int should_split_arg(char *arg, char *original_arg)
{
    char *equals;
    char *orig_equals = NULL;
    
    if (!arg || !*arg)
        return 0;
    int is_append = pls_conter(original_arg);

    printf("is_append should split arg %d\n", is_append);
    
    if (is_append == 1 )
        return 0;
    
    if (strchr(arg, '$'))
        return 1;
    
    equals = strchr(arg, '=');
    if (!equals)
        return 0; 
    if (!is_valid_var_name(arg, equals - arg))
        return 1;
    
    if (original_arg) 
    {
        orig_equals = strchr(original_arg, '=');
        if (orig_equals && check_var_quotes(original_arg, orig_equals))
            return 1;
    }
    return 0; 
}


char **split_if_needed(char *str)
{
    int i = 0;
    int count;
    char **result;
    char *trimmed_str;

    if (!str || !*str)
        return NULL;
    trimmed_str = ft_strtrim(str, " ");
    if (!trimmed_str)
        return NULL;
    if (!strchr(trimmed_str, ' ') && !strchr(trimmed_str, '\t') && !strchr(trimmed_str, '\n'))
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
    return result;
}


void cmd_splitting_helper(t_cmd *current, char **new_args, char **split, int word_count, int arg_count)
{
    int j;

    new_args[0] = strdup(split[0]);
    j = 1;
    while (j < word_count) 
        {
            new_args[j] = strdup(split[j]);
                    j++;
        }
        j = 1;
        while (j < arg_count) 
            {
                new_args[j+word_count-1] = current->args[j];
                    j++;
            }
            new_args[arg_count+word_count-1] = NULL;
            j = 0;
                current->args = new_args;
}

void prepare_new_args(char **new_args, t_cmd *current, int i)
{
    int j = 0;
    
    while (j < i)
    {
        new_args[j] = current->args[j];
        j++;
    }
    free(current->args[i]);
}

int rebuild_cmd_args(char **new_args, t_cmd *current, char **split, 
                    int i, int word_count)
{
    int j;
    int arg_count;
    
    j = 1;
    arg_count = ft_lint(current->args);
    new_args[i] = strdup(split[0]);
    while (j < word_count)
    {
        new_args[i+j] = strdup(split[j]);
        j++;
    }
    j = i+1;
    while (j < arg_count)
    {
        new_args[j+word_count-1] = current->args[j];
        j++;
    }
    new_args[arg_count+word_count-1] = NULL;
    free(current->args);
    current->args = new_args;
    
    return i + word_count - 1;
}
