#include "parsing.h"
#include <ctype.h>
#define MAX_BUFFER_SIZE (1ULL << 30) // 1GB max buffer size

int var_name_extract(t_exp_helper *expand)
{

    if (ft_isdigiti(expand->original[expand->i]))
    {
        expand->i++;
        return (2);
    }
    else if ((expand->original[expand->i] == '"' || expand->original[expand->i] == '\'') && 
                     expand->quote_state == 0) // $ is not inside any quotes
    {
        if (expand->i >= 2 && expand->original[expand->i - 2] == '$') 
            {
                if (!ensure_buffer_space(expand, 1))
                    return (0);
                expand->expanded[expand->j++] = '$';
                return (1);
            }
                return (1);
    }
    else 
    {
        while (expand->original[expand->i] && is_valid_var_char(expand->original[expand->i]))
            expand->i++;
        return (2);
    }
}

int alloc_var_name(t_exp_helper *expand)
{

    size_t var_len;

    var_len = expand->i - expand->start;
    if (var_len > SIZE_MAX - 1)
    {
                fprintf(stderr, "minishell: memory allocation failed: variable name too long\n");
                return (0);
    }
    if (var_len == 0) 
    {
        if (!ensure_buffer_space(expand, 1)) 
            return (0);
        expand->expanded[expand->j++] = '$';
        return (1);
    }
    expand->var_name = malloc(var_len + 1);
    if (!expand->var_name)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        exit(1);
    }
    memcpy(expand->var_name, expand->original + expand->start, var_len);
    expand->var_name[var_len] = '\0';
    return (2);
}
void value_extracter(t_exp_helper *expand, t_env *env)
{
    char *var = NULL;
    char *lk_var = NULL;
    if (is_valid_key(expand->var_name) != 1)
        {
        lk_var = lookup_variable(expand->var_name, env);
        var = chenger(trim_whitespace((lk_var)));
        free(lk_var);
    }
    if (var != NULL)
        expand->var_value = var;
    free(expand->var_name);
    expand->var_name = NULL;
}
int adding_var_value(t_exp_helper *expand)
{

    size_t len = strlen(expand->var_value);
    if (len > SIZE_MAX - expand->j)
    {
        fprintf(stderr, "minishell: memory allocation failed: buffer overflow\n");
                free(expand->var_value);
                expand->var_value = NULL;
                return (0);
    }
    if (!ensure_buffer_space(expand, len))
    {
        free(expand->var_value);
         expand->var_value = NULL;
        return (0);
    }
    memcpy(expand->expanded + expand->j, expand->var_value, len);
    expand->j += len;
    free(expand->var_value);
    expand->var_value = NULL;
    return (1);

}

int extracting_the_key_value(t_exp_helper *expand, int exit_status, t_env *env, int pipe_out)
{
    int extract_result;
    int alloc_result;
    if (helper3(expand, exit_status, pipe_out) == 0)
        {
            expand->start = expand->i;
            extract_result = var_name_extract(expand);
            if (extract_result == 1)
                return (1);
            else if (extract_result == 0)
                return (0);
            alloc_result = alloc_var_name(expand);
            if (alloc_result == 0)
                return (0);
            else if (alloc_result == 1)
                return (1);
            value_extracter(expand, env);
            return (2);
        }
    return (1);

}
int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env, int pipe_out)
{
    char *var;
    int extracting;
    size_t var_len;
    
    int res_adding_var;
    var = NULL;
    if (expand->original[expand->i] == '$' && expand->quote_state != 1) // Not in single quotes
    {
        expand->i++;
        extracting = extracting_the_key_value(expand, exit_status, env, pipe_out);
        if (extracting == 0)
            return 0;
        if (expand->var_value)
        {
           res_adding_var = adding_var_value(expand);
           if (res_adding_var == 0)
                return 0;
        }
        else if (extracting == 1)
            return 1;
        return (1);
    }
    return (0);
}

void process_string(char *str, t_exp_helper *expand, t_env *env, int exit_status, int pipe_out)
{
	if (!expand_fill_str(expand, str))
	{
		expand->expanded = NULL;
		return;
	}
	while (expand->original[expand->i])
	{
		if (!expand_handle_helper0(expand) && !expand_handle_helper1(expand, exit_status, env, pipe_out))
		{
			if (!ensure_buffer_space(expand, 1))
			{
				free(expand->expanded);
				expand->expanded = NULL;
				break;
			}
			expand->expanded[expand->j++] = expand->original[expand->i++];
		}
	}
    expand->expanded[expand->j] = '\0';
}


t_exp_helper *alloc_expand()
{

    t_exp_helper *expand = malloc(sizeof(t_exp_helper));
    if (!expand)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        exit(1);
    }
    expand->buffer_size = 0;
    expand->expanded = NULL;
    expand->var_name = NULL;
    expand->var_value = NULL;
    expand->had_removed_var = 0;
    return expand;
}

void case_handle(t_cmd *current, int *i, int *pls_case, int *should_split)
{
    if (pls_conter(current->args[(*i)]) == 1)
                (*pls_case) = 1;
    if (((current->args[(*i)][0] == '\'' && current->args[(*i)][strlen(current->args[(*i)]) - 1] == '\'') ||
            (current->args[(*i)][0] == '"' && current->args[(*i)][strlen(current->args[(*i)]) - 1] == '"')) && current->args[(*i) + 1] != NULL)
               (*should_split) = 0;
    else
    {
        if (strchr(current->args[(*i)], '$'))
            (*should_split) = 1;
        else 
            (*should_split) = 1;
    } 

}

void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
    t_cmd *current;
    t_exp_helper *expand;
    t_redir *redir;
    int i;
    int should_split;
    int pls_case;
    int had_empty_var;

    should_split = 0;
    pls_case = 0;
    had_empty_var = 0;
    expand = alloc_expand();
    current = cmd_list;
    while (current)
    {
        should_split = 0;
        i = 0;
        while (current->args && current->args[i])
        {
            case_handle(current, &i, &pls_case, &should_split);
            process_string(current->args[i], expand, env, exit_status, cmd_list->pipe_out);
            if (expand->expanded && expand->expanded[0] == '\0' && 
                strchr(current->args[i], '$'))
            {
                free(current->args[i]);
                free(expand->expanded);
                expand->expanded = NULL;
                if (i == 0)
                    expand->had_removed_var = 1;
                else
                    expand->had_removed_var = 0;
                if (current->args_befor_quotes_remover && current->args_befor_quotes_remover[i])
                {
                    free(current->args_befor_quotes_remover[i]);
                    int j = i;
                    while (current->args_befor_quotes_remover[j + 1])
                    {
                        current->args_befor_quotes_remover[j] = current->args_befor_quotes_remover[j + 1];
                        j++;
                    }
                    current->args_befor_quotes_remover[j] = NULL;
                }
                int j = i;
                while (current->args[j + 1])
                {
                    current->args[j] = current->args[j + 1];
                    j++;
                }
                current->args[j] = NULL;
            }
            else
            {
                free(current->args[i]);
                current->args[i] = expand->expanded;
                expand->expanded = NULL;
                i++;
            }
        }
        if (current->args && current->args[0]) 
        {
            if (current->cmd)
                free(current->cmd);
            current->cmd = ft_strdup(current->args[0]);
            if (!current->cmd)
            {
                fprintf(stderr, "minishell: memory allocation failed\n");
                exit(1);
            }
        }
        redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                if (redir->type != 3)
                {
                process_string(redir->file, expand, env, exit_status, cmd_list->pipe_out);
                free(redir->file);
                redir->file = expand->expanded;
                expand->expanded = NULL;
                }
            }
            redir = redir->next;
        }
        current = current->next;
    }
    if (pls_case == 1 && should_split == 1) 
        should_split = 0;
    if (should_split | had_empty_var)
        apply_word_splitting(cmd_list, expand);
    else
        free_expanded(expand);
}
