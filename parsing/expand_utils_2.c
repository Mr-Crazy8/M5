/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:21:58 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/09 11:31:50 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"



// int is_var_key_append(char *original_arg)
// {
//     char *dollar_pos;
//     char *plus_equals_pos;
    
//     if (!original_arg)
//         return 0;
    
//     // Find the first $ character (ignore any quotes before it)
//     dollar_pos = strchr(original_arg, '$');
//     if (!dollar_pos)
//         return 0;
    
//     // Find the += pattern
//     plus_equals_pos = strstr(original_arg, "+=");
//     if (!plus_equals_pos)
//         return 0;
    
//     // Make sure $ comes before +=
//     if (dollar_pos < plus_equals_pos)
//         return 1;
    
//     return 0;
// }
int is_var_key_append(char *original_arg)
{
    char *dollar_pos;
    char *plus_equals_pos;
    
    if (!original_arg)
        return 0;
    
    // Find the first $ character (anywhere in the string, not just at beginning)
    dollar_pos = strchr(original_arg, '$');
    if (!dollar_pos)
        return 0;
    
    // Find the += pattern
    plus_equals_pos = strstr(original_arg, "+=");
    if (!plus_equals_pos)
        return 0;
    
    // Make sure $ comes before +=
    if (dollar_pos < plus_equals_pos)
        return 1;
    
    return 0;
}

int key_is_var(char *str)
{
    if (!str || str[0] == '\0')
     return 0;
    if (str[0] == '$')
         return 1;
    else if ((str[0] == '\'' && str[1] == '\'') && str[2] == '$')
        return 1;
    else if ((str[0] == '\"' && str[1] == '\"') && str[2] == '$')
        return 1;
    return (0);
}

int has_quotes_before_plus(char *str)
{
    char *equals_pos;
    char *plus_pos;
    int j;
    
    if (!str)
        return 0;
    
    equals_pos = strchr(str, '=');
    if (!equals_pos || equals_pos == str)
        return 0;
    if (*(equals_pos - 1) == '+') 
    {
        plus_pos = equals_pos - 1;
        j = 0;
        while (j < (plus_pos - str))
        {
            if (str[j] == '\'' || str[j] == '"') 
                return 1;
            j++;
        }
    }
    
    return 0;
}



int is_append_assignment(char *str)
{
    char *equals_pos;
    char *ptr;
    int plus_count = 0;
    
    if (!str)
        return 0;
    
    if (key_is_var(str) || has_quotes_before_plus(str))
        return 0;
    
    equals_pos = strchr(str, '=');
    if (!equals_pos)
        return 0;
    if (equals_pos > str && *(equals_pos - 1) == '+')
    {
        ptr = equals_pos - 1;
        while (ptr >= str && *ptr == '+')
        {
            plus_count++;
            ptr--;
        }
        if (plus_count == 1)
            return 1;
    }
    
    return 0;
}


int should_split_arg(char *arg, char *original_arg)
{
    char *equals;
    char *orig_equals = NULL;
    
    if (!arg || !*arg)
        return 0;
    
    // Check for variable key with append format (e.g. $f+=value or ""$f+=value)
    // We want to ensure this triggers splitting
    int app = is_var_key_append(original_arg);
    if (original_arg && app)
        return 1; // Force split
        
    // Handle other cases as before
    if (original_arg && key_is_var(original_arg))
        return 1;
        
    if (original_arg && has_quotes_before_plus(original_arg))
        return 1;
        
    if (original_arg && is_append_assignment(original_arg))
        return 0;
        
    if (is_append_assignment(arg))
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

// int should_split_arg(char *arg, char *original_arg)
// {
//     char *equals;
//     char *orig_equals = NULL;
    
//     if (!arg || !*arg)
//         return 0;
//     if (original_arg && (key_is_var(original_arg) == 1))
//         return 1;
//     if (original_arg && has_quotes_before_plus(original_arg))
//         return 1;
//      if (original_arg && is_var_key_append(original_arg))
//         return 1;
//     if (original_arg && is_append_assignment(original_arg) && (key_is_var(original_arg) == 1))
//         return 0;
//     if (is_append_assignment(arg))
//         return 0;
//     if (strchr(arg, '$'))
//         return 1;
//     equals = strchr(arg, '=');
//     if (!equals)
//         return 0; 
//     if (!is_valid_var_name(arg, equals - arg))
//         return 1;
//     if (original_arg) 
//     {
//         orig_equals = strchr(original_arg, '=');
//         if (orig_equals && check_var_quotes(original_arg, orig_equals))
//             return 1;
//     }
    
//     return 0; 
// }



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
