/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:16:14 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/09 12:32:08 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void cmd_splitting(t_cmd *cmd_list )
{
     t_cmd *current = cmd_list;
     int word_count;
     int arg_count;
     char **new_args;
     char **split;
     if (current->args && current->args[0] && current->cmd &&
            strcmp(current->args[0], current->cmd) == 0) 
        {
            split = split_if_needed(current->cmd);
            if (split && split[1]) 
            { 
                free(current->cmd);
                current->cmd = strdup(split[0]);
                word_count = ft_lint(split);
                arg_count = ft_lint(current->args);
                new_args = malloc(sizeof(char *) * (arg_count + word_count));
                if (new_args)
                    cmd_splitting_helper(current, new_args, split, word_count, arg_count);
            }
            if (split)
                free_string_array(split);
        } 
}

void  apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand)
{
    t_cmd *current = cmd_list;
    int should_split;
    

    while (current)
    {
        cmd_splitting(current);
        
        should_split = 1;
        
        if (current->cmd && strcmp(current->cmd, "export") == 0) 
        {
            should_split = is_special_export_case(current);
        }
        split_the_rest(current, should_split, expand->had_removed_var);
        print_cmd(current);
        current = current->next;
    }
    free_expanded(expand);
}
