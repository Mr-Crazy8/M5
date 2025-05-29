//expand_utils.c

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


int should_split_arg(char *arg, char *original_arg)
{
    char *equals;
    char *orig_equals = NULL;
    
    if (!arg || !*arg)
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



// In expand_utils.c, modify split_if_needed function to better handle leading spaces:

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




void free_string_array(char **array)
{
    if (!array)
        return;
    int i = 0;
    while (array[i])
        {
            free(array[i]);
            i++;
        }
    
    free(array);
}


int check_var_quotes(char *orig_arg, char *orig_equals)
{
    int j = 0;
    

    while (j < (orig_equals - orig_arg))
    {
        if (orig_arg[j] == '\'' || orig_arg[j] == '"' || orig_arg[j] == '$') 
            return 1;  // Variable name has quotes or $ - split
        j++;
    }
    // Check if variable name starts with a digit or invalid character
    if (((isdigit(orig_arg[0]) || (!isalpha(orig_arg[0]) && orig_arg[0] != '_'))))
        return 1;
        
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



void split_the_rest_helper(char *equals, int should_split, t_cmd *current, int *i)
{
    char **split;
    int word_count;
    char **new_args;
    int force_split = 0;

    if (current->args[(*i)] && equals)
    {
        if (isdigit(current->args[(*i)][0]))
            force_split = 1;
        if (current->args_befor_quotes_remover) 
        {
            int count = 0;
            while (current->args_befor_quotes_remover[count])
                count++;
            if (*i < count && current->args_befor_quotes_remover[*i]) 
            {
                char *orig_equals = strchr(current->args_befor_quotes_remover[*i], '=');
                if (orig_equals && check_var_quotes(current->args_befor_quotes_remover[*i], orig_equals))
                    force_split = 1;
            }
        }
    }

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



void apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand)
{
    t_cmd *current = cmd_list;
    int should_split;
    
    while (current)
    {
        cmd_splitting(current);
        
        should_split = 1;
        
        if (current->cmd && strcmp(current->cmd, "export") == 0) {
            should_split = is_special_export_case(current);
        }
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
