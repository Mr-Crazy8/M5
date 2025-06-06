/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:42:21 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/05 22:47:44 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void split_the_rest_helper_01(t_cmd *current, char *equals, int *i, int force_split, int should_split)
{
    char **split;
    int word_count;
    char **new_args;
     
    if (!current->args[(*i)] || (!should_split && !force_split))
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

void split_the_rest_helper(char *equals, int should_split, t_cmd *current, int *i)
{
    int force_split;
    int count;
    char *orig_equals;
    int append = pls_conter(current->args_befor_quotes_remover[*i]);
    printf("append  010 ===== %d\n", append);
    force_split = 0;
    if (current->args[(*i)] && equals)
    {
        if (isdigit(current->args[(*i)][0]))
            force_split = 1;
        if (current->args_befor_quotes_remover) 
        {
            count = 0;
            while (current->args_befor_quotes_remover[count])
                count++;
            if (*i < count && current->args_befor_quotes_remover[*i]) 
            {
                orig_equals = strchr(current->args_befor_quotes_remover[*i], '=');
                if (orig_equals && check_var_quotes(current->args_befor_quotes_remover[*i], orig_equals) && pls_conter(current->args_befor_quotes_remover[*i]))
                    force_split = 1;
            }
        }
    }
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

    if (split_the_rest_hp(current, &should_split, &i))
        return;
    i = 1; 
    while (current->args && current->args[i]) 
    {
        equals = strchr(current->args[i], '=');
        if (had_removed_var == 1)
            arg_should_split = 1;
        else if (!equals && current->args_befor_quotes_remover && i < ft_lint(current->args_befor_quotes_remover) && 
            current->args_befor_quotes_remover[i] && strchr(current->args_befor_quotes_remover[i], '$')) 
            arg_should_split = 1; 
        else if (current->args_befor_quotes_remover && i < ft_lint(current->args_befor_quotes_remover)) 
            arg_should_split = should_split_arg(current->args[i], current->args_befor_quotes_remover[i]);
        else
            arg_should_split = should_split_arg(current->args[i], NULL);
        split_the_rest_helper(equals, arg_should_split, current, &i);
        i++;
    }
}

