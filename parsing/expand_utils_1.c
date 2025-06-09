/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:17:36 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/09 14:50:06 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_valid_var_name(char *str, int len)
{
    int i = 1;
    if (!str || len <= 0)
        return 0;
    if ((!isalpha(str[0]) && str[0] != '_'))
        return 0;
    while (i < len)
    {
        if (!is_valid_var_char(str[i]))
            return 0;
        i++;
    }
    return 1;
}

int check_var_quotes(char *orig_arg, char *orig_equals)
{
    int j = 0;
    // int is_append = pls_conter(orig_arg);
    // printf("orig_arg === %s\n", orig_arg);
    // printf("is_append === %d\n", is_append);
    // if (is_append)
    //     return 0;
    while (j < (orig_equals - orig_arg))
    {
        if ((orig_arg[j] == '\'' || orig_arg[j] == '"' || orig_arg[j] == '$')) 
            return 1;  // Variable name has quotes or $ - split

        j++;
    }
    // Check if variable name starts with a digit or invalid character
    if ((((isdigit(orig_arg[0]) || (!isalpha(orig_arg[0]) && orig_arg[0] != '_')))))
        return 1;
        
    return 0;
}



int ft_lint(char **str)
{
    int i = 0;
    while (str && str[i])
        i++;
    return i;
}


int is_special_export_case(t_cmd *cmd)
{

    if (!cmd || !cmd->cmd || !cmd->args || !cmd->args[0])
        return 0;
        
    if (strcmp(cmd->cmd, "export") != 0)
        return 0;  // Not export command
    
    return 1;  // It's export command, we'll check each argument individually
}


int check_export_cmd(t_cmd *cmd)
{
    if (!cmd || !cmd->cmd || !cmd->args || !cmd->args[0])
        return 0;
    if (strcmp(cmd->cmd, "export") != 0)
        return 1;  
    if (cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[0]) 
    {
        char *orig_cmd = cmd->args_befor_quotes_remover[0];
        if (strcmp(orig_cmd, "export") != 0) 
            return 1;  // Had quotes or variables - split
    }
    return 2;  // Continue checking
}