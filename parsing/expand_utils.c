/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:16:14 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 10:26:29 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.h"
void print_old(char **str);
char **copy_string_array(char **array);
void free_string_array_partial(char **array, int count);

int is_valid_var_name(char *str, int len)
{
    int i = 0;
    if (!str || len <= 0)
        return 0;
 
    if ((!isalpha(str[0]) && str[0] != '_'))
        return 0;
    // if (str[len - 1] == '+')
    //     len -= 1;
    while (i < len)
    {
        if (is_valid_var_char(str[i]) != 1)
            return 0;
        i++;
    }
    return 1;
}

int should_split_arg(char *arg, char *original_arg)
{
    char *equals;
    char *plus_equals;
    char *orig_equals = NULL;
    char *orig_plus_equals = NULL;
    int is_append = 0;
    
    if (!arg || !*arg)
        return 0;
    if (strchr(arg, '$'))
    {
        printf("100 should_split_arg\n");
        return 1;
    }
    equals = strchr(arg, '=');
    plus_equals = strstr(arg, "+=");
    if (!equals)
    {   
        printf("107 should_split_arg\n");
        return 0; 
    }
    if (plus_equals && plus_equals < equals)
    {
        is_append = 1;
        if (!is_valid_var_name(arg, plus_equals - arg))
        {
            printf("append var name invalid should_split_arg\n");
            return 1;
        }
    }
    else 
    {
        if (!is_valid_var_name(arg, equals - arg))
        {   
            printf("112 should_split_arg\n");
            return 1;
        }
    }
    if (original_arg) 
    {
        orig_equals = strchr(original_arg, '=');
        orig_plus_equals = strstr(original_arg, "+=");
        if (!is_append && orig_equals && check_var_quotes(original_arg, orig_equals))
        {
            printf("121 should_split_arg\n");
            return 1;
        }
        if (is_append && orig_plus_equals && 
            orig_plus_equals < orig_equals &&
            check_var_quotes(original_arg, orig_plus_equals))
        {
            printf("append with quotes/$ should_split_arg\n");
            return 1;
        }
    }
   if (!is_append && strchr(equals + 1, ' ') && 
        original_arg && check_var_quotes(original_arg, strchr(original_arg, '=')))
        return 1;
    printf("125 should_split_arg\n");
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

int check_var_quotes(char *orig_arg, char *orig_equals)
{
    int j = 0;
    
    if (!orig_arg || !orig_equals)
        return 0;
        
    // Check only the key part before equals sign
    while (j < (orig_equals - orig_arg))
    {
        if (orig_arg[j] == '\'' || orig_arg[j] == '"' || orig_arg[j] == '$') 
        {
            return 1;  // Variable name has quotes or $ - split
        }
        j++;
    }
    
    // Check if variable name starts with a digit or invalid character
    if (((isdigit(orig_arg[0]) || (!isalpha(orig_arg[0]) && orig_arg[0] != '_'))))
    {
        return 1;
    }
        
    return 0;
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



int is_special_export_case(t_cmd *cmd)
{
    if (!cmd || !cmd->cmd || !cmd->args || !cmd->args[0])
        return 0;
        
    if (strcmp(cmd->cmd, "export") != 0)
        return 0;  // Not export command
        
    return 1;  // It's export command, we'll check each argument individually
}


int ft_lint(char **str)
{
    int i = 0;
    while (str && str[i])
        i++;
    return i;
}




void cmd_splitting_helper(t_cmd *current, char **new_args, char **split, int word_count, int arg_count)
{
    int j;
    char **old_args = current->args;

    // Add all split words
    j = 0;
    while (j < word_count) 
    {
        new_args[j] = strdup(split[j]);
        j++;
    }
    
    j = 1;
    while (j < arg_count) 
    {
        new_args[word_count + j - 1] = old_args[j];
        j++;
    }
    
    new_args[arg_count + word_count - 1] = NULL;
    current->args = new_args;
    free(old_args); 
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




void split_the_rest_helper(char *equals, int should_split, t_cmd *current, int *i)
{
    char **split;
    int word_count;
    char **new_args;
    int force_split = 0;
    char *plus_equals;
    int is_append = 0;
    int is_export_cmd = 0;

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

int  split_the_rest_hp(t_cmd *current, int *should_split, int *i)
{
    char *equals;
    if (!current->args)
        return 0;
    if (!current->cmd || strcmp(current->cmd, "export") != 0) 
    {
        (*i) = 0;
        while (current->args && current->args[(*i)]) 
        {
            equals = strchr(current->args[(*i)], '=');
            split_the_rest_helper(equals, (*should_split), current, i);
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
        split_the_rest_helper(equals, arg_should_split, current, &i);
        i++;
    }
}




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

void print_old(char **str)  /// remove
{
    int i = 0;

    while (str[i])
    {
        printf("print_old____[%s]___\n", str[i]);
        i++;
    }
}
int is_quote(char c) /// remove
{
    return (c == '\'' || c == '"');
}

// Function to fix quotes in export variable names
int fix_export_var_names(t_cmd *cmd)   /// remove
{
    if (!cmd || !cmd->cmd || !cmd->args_befor_quotes_remover || 
        strcmp(cmd->cmd, "export") != 0)
        return 0;
    
    int i = 1;  // Skip command name (export)
    int modified = 0;
    
    // Process each argument
    while (cmd->args_befor_quotes_remover[i])
    {
        char *arg = cmd->args_befor_quotes_remover[i];
        char *equals = strchr(arg, '=');
        
        // If no equals sign or equals is at beginning, skip
        if (!equals || equals == arg)
        {
            i++;
            continue;
        }
        
        // Check for quotes in variable name
        int j = 0;
        int quote_pos = -1;
        char quote_char = '\0';
        
        while (j < (equals - arg))
        {
            if (is_quote(arg[j]))
            {
                quote_pos = j;
                quote_char = arg[j];
                break;
            }
            j++;
        }
        
        // If we found a quote, fix it
        if (quote_pos != -1)
        {
            int arg_len = strlen(arg);
            char *new_arg = malloc(arg_len + 2);  // +2 for safety
            
            if (new_arg)
            {
                // Copy part before the quote
                j = 0;
                while (j < quote_pos)
                {
                    new_arg[j] = arg[j];
                    j++;
                }
                
                // Copy part between quote and equals
                int src_idx = quote_pos + 1;
                int dest_idx = quote_pos;
                
                while (src_idx < (equals - arg))
                {
                    new_arg[dest_idx] = arg[src_idx];
                    dest_idx++;
                    src_idx++;
                }
                
                // Add equals sign
                new_arg[dest_idx] = '=';
                dest_idx++;
                
                // Add the quote
                new_arg[dest_idx] = quote_char;
                dest_idx++;
                
                // Copy the rest of the string (after equals)
                src_idx = (equals - arg) + 1;  // Skip original equals
                while (arg[src_idx])
                {
                    new_arg[dest_idx] = arg[src_idx];
                    dest_idx++;
                    src_idx++;
                }
                new_arg[dest_idx] = '\0';
                
                // Replace original with fixed version
                free(cmd->args_befor_quotes_remover[i]);
                cmd->args_befor_quotes_remover[i] = new_arg;
                modified = 1;
            }
        }
        
        i++;
    }
    
    return modified;
}

void apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand)
{
    t_cmd *current = cmd_list;
    int should_split = 0;
    
    while (current)
    {
        split_the_rest(current, should_split, expand->had_removed_var);
        current = current->next;
    }
     if (expand) 
     {
        if (expand->expanded)
            free(expand->expanded);
        if (expand->var_name)
            free(expand->var_name);
        if (expand->var_value)
            free(expand->var_value);
        free(expand);
    }
}
