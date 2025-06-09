/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:42:21 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/09 11:09:32 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void split_the_rest_helper_01(t_cmd *current, char *equals, int *i, int force_split, int should_split)
{
    char **split;
    int word_count;
    char **new_args;
    int count = 0;
    
    if (!current->args[(*i)] || (!should_split && !force_split))
        return;
        
    if (current->args_befor_quotes_remover) 
    {
        while (current->args_befor_quotes_remover[count])
            count++;
    }
    
    if (current->args_befor_quotes_remover && *i < count && 
        current->args_befor_quotes_remover[*i] && 
        is_var_key_append(current->args_befor_quotes_remover[*i]))
    {
        // Force splitting for all variable key append assignments
        force_split = 1;
        should_split = 1;
    }
    // Handle regular append assignment (not variable key) - don't split
    else if (current->args_befor_quotes_remover && *i < count && 
        current->args_befor_quotes_remover[*i] && 
        is_append_assignment(current->args_befor_quotes_remover[*i]) && 
        !is_var_key_append(current->args_befor_quotes_remover[*i]))
        return;
    
    if (is_append_assignment(current->args[*i]) && 
        !(current->args_befor_quotes_remover && *i < count && 
          current->args_befor_quotes_remover[*i] && 
          is_var_key_append(current->args_befor_quotes_remover[*i])))
        return;
    
    if (!equals || should_split || force_split)
    {
        split = split_if_needed(current->args[(*i)]);
        if (split && split[1])
        {
            word_count = ft_lint(split);
            new_args = malloc(sizeof(char *) * 
                       (ft_lint(current->args) + word_count));
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
}


// Updated split_the_rest_helper to ensure it correctly processes variable append operations
void split_the_rest_helper(char *equals, int should_split, t_cmd *current, int *i)
{
    int force_split = 0;
    int count = 0;
    char *orig_equals;
    
    // Count original arguments
    if (current->args_befor_quotes_remover) 
    {
        while (current->args_befor_quotes_remover[count])
            count++;
    }
    
    // Check for explicit $var+= pattern
    if (current->args_befor_quotes_remover && *i < count && 
        current->args_befor_quotes_remover[*i] &&
        current->args_befor_quotes_remover[*i][0] == '$'
        && strstr(current->args_befor_quotes_remover[*i], "+="))
    {
        // Always force split for variable appends
        force_split = 1;
        should_split = 1;
    }
    else if (current->args_befor_quotes_remover && *i < count && 
        current->args_befor_quotes_remover[*i] &&
        current->args_befor_quotes_remover[*i][0] == '$'
        && strstr(current->args_befor_quotes_remover[*i], "+="))
    {
        // Always force split for variable appends
        force_split = 1;
        should_split = 1;
    }
    else if (current->args_befor_quotes_remover && *i < count && 
             current->args_befor_quotes_remover[*i] &&
             strstr(current->args_befor_quotes_remover[*i], "+=") &&
             current->args_befor_quotes_remover[*i][0] != '$')
    {
        // Skip regular append assignments
        return;
    }
    
    if (current->args[(*i)] && equals)
    {
        if (isdigit(current->args[(*i)][0]))
            force_split = 1;
            
        if (current->args_befor_quotes_remover && *i < count && 
            current->args_befor_quotes_remover[*i]) 
        {
            orig_equals = strchr(current->args_befor_quotes_remover[*i], '=');
            if (orig_equals && check_var_quotes(current->args_befor_quotes_remover[*i], orig_equals))
                force_split = 1;
        }
    }
    
    // Call helper with the determined flags
    split_the_rest_helper_01(current, equals, i, force_split, should_split);
}


int  split_the_rest_hp(t_cmd *current, int *should_split, int *i)
{
    char *equals;
    int was_variable = 0;
    if (!current->args)
        return 0;
    if (current->args_befor_quotes_remover && 
        current->args_befor_quotes_remover[0] && 
        strchr(current->args_befor_quotes_remover[0], '$')) {
        was_variable = 1;
    }
    if (!current->cmd || strcmp(current->cmd, "export") != 0 || was_variable) 
    {
        (*i) = 0;
        while (current->args && current->args[(*i)]) 
        {
            equals = strchr(current->args[(*i)], '=');
              printf("\n");
            split_the_rest_helper(equals, (*should_split), current, &(*i));
            (*i)++;
        }
        return 1;
    }
    return 0;
}






void split_the_rest(t_cmd *current, int should_split, int had_removed_var)
{
    int i;
    char *equals;
    int arg_should_split;
    int count = 0;
    if (split_the_rest_hp(current, &should_split, &i))
        return;
        
    if (current->args_befor_quotes_remover) 
    {
        while (current->args_befor_quotes_remover[count])
            count++;
    }
    
    i = 1; 
    while (current->args && current->args[i]) 
    {
        // CRITICAL FIX: Check for $var+= pattern explicitly to fix the problem
        int is_var_append = 0;
        if (current->args_befor_quotes_remover && i < count && 
            current->args_befor_quotes_remover[i])
        {
            char *arg = current->args_befor_quotes_remover[i];
            if (is_var_key_append(arg))
            {
                char *plus_eq = strstr(arg, "+=");
                if (plus_eq)
                {
                    is_var_append = 1;
                }
            }
        }
        if (is_var_append)
        {
            equals = strchr(current->args[i], '=');
            split_the_rest_helper(equals, 1, current, &i); 
            i++;
            continue;
        }
        
        // Skip regular append assignments that aren't variable keys
        if ((current->args_befor_quotes_remover && i < count &&
            current->args_befor_quotes_remover[i] && 
            strstr(current->args_befor_quotes_remover[i], "+=") &&
            current->args_befor_quotes_remover[i][0] != '$'))
        {
            i++;
            continue;
        }
        
        // Normal case - not a variable append or regular append
        equals = strchr(current->args[i], '=');
        
        if (had_removed_var == 1)
            arg_should_split = 1;
        else if (!equals && current->args_befor_quotes_remover && i < count && 
                 current->args_befor_quotes_remover[i] && 
                 strchr(current->args_befor_quotes_remover[i], '$')) 
            arg_should_split = 1; 
        else if (current->args_befor_quotes_remover && i < count) 
        {
            arg_should_split = should_split_arg(current->args[i], 
                                               current->args_befor_quotes_remover[i]);
        }
        else
        {
            arg_should_split = should_split_arg(current->args[i], NULL);
        }
        
        // Process the argument with the determined should_split value
        split_the_rest_helper(equals, arg_should_split, current, &i);
        i++;
    }
}