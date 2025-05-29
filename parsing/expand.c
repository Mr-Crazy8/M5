#include "parsing.h"
#include <ctype.h>
#define MAX_BUFFER_SIZE (1ULL << 30) // 1GB max buffer size

// char *convert_to_one_space(char *str)
// {
    
// }



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

int ensure_buffer_space_hp(t_exp_helper *expand)
{
    if (!expand->buffer_size)
	{
		size_t original_len = strlen(expand->original);
		if (original_len > SIZE_MAX / 2 - 1024)
		{
            write(2, "minishell: memory allocation failed: input too large\n" , 54);
			return (0);
		}
		expand->buffer_size = original_len * 2 + 1024;
	}
    return 1;
}
int ensure_buffer_space(t_exp_helper *expand, size_t additional_needed)
{
    char *new_buffer;
    size_t new_size;

    if (!ensure_buffer_space_hp(expand))
        return (0);
	if (expand->j + additional_needed >= expand->buffer_size)
	{
		new_size = expand->buffer_size * 2;
		if (new_size < expand->j + additional_needed + 1)
			new_size = expand->j + additional_needed + 1024;
		new_buffer = calloc(1, new_size);
		if (!new_buffer)
		{
            write(2, "minishell: memory allocation failed\n", 37);
			exit(1);
        }
		if (expand->expanded)
		{
			memcpy(new_buffer, expand->expanded, expand->j);
			free(expand->expanded);
		}
		expand->expanded = new_buffer;
		expand->buffer_size = new_size;
	}
	return (1);
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

int	ft_isdigiti(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	else
		return (0);
}




static int	is_in_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i2;
	size_t	i1;
	size_t	r;
	size_t	len;
	char	*strtrim;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	i1 = 0;
	i2 = ft_strlen(s1);
	r = 0;
	while (s1[i1] != '\0' && is_in_set(s1[i1], set))
		i1++;
	while (i2 > i1 && is_in_set(s1[i2 - 1], set))
		i2--;
	len = i2 - i1;
	strtrim = (char *)malloc((len + 1) * sizeof(char));
	if (strtrim == NULL)
		return (NULL);
	while (i1 < i2)
		strtrim[r++] = s1[i1++];
	strtrim[r] = '\0';
	return (strtrim);
}



char *chenger(char *str)
{
    int i = 0;

    if (!str)
        return NULL;
    while (str[i])
    {
        if (str[i] == '\'')
            str[i] = 10;
        else if (str[i] == '\"')
            str[i] = 11;
        i++;
    }
    return (str);

}

int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env, int pipe_out)
{
    char *var;
    var = NULL;
    if (expand->original[expand->i] == '$' && expand->quote_state != 1) // Not in single quotes
    {
        expand->i++;
        if (helper3(expand, exit_status, pipe_out) == 0)
        {
            expand->start = expand->i;
            if (ft_isdigiti(expand->original[expand->i])) 
            {
                expand->i++;
            } 
            // Handle case where $ is followed by a quote (like $"HOME" or $'HOME')
            // But ONLY if the $ itself is not inside double quotes
            else if ((expand->original[expand->i] == '"' || expand->original[expand->i] == '\'') && 
                     expand->quote_state == 0) // $ is not inside any quotes
            {
                // Check if it's $$"HOME" case by looking at the character before the first $
                if (expand->i >= 2 && expand->original[expand->i - 2] == '$') 
                {
                    // This is $$"HOME" case, don't remove the $
                    if (!ensure_buffer_space(expand, 1)) {
                        return (0);
                    }
                    expand->expanded[expand->j++] = '$';
                    return (1);
                }
                // Single $ followed by quote and $ is not in quotes - remove the $ and continue
                // Don't increment i, let the quote be processed normally
                return (1);
            }
            else {
                while (expand->original[expand->i] && is_valid_var_char(expand->original[expand->i]))
                    expand->i++;
            }
            
            size_t var_len = expand->i - expand->start;
            if (var_len > SIZE_MAX - 1)
            {
                fprintf(stderr, "minishell: memory allocation failed: variable name too long\n");
                return (0);
            }
            if (var_len == 0) {
                if (!ensure_buffer_space(expand, 1)) {
                    return (0);
                }
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
            if (is_valid_key(expand->var_name) != 1)
            {
                // var = ft_strtrim(chenger(lookup_variable(expand->var_name, env)), " ");
                var = chenger(lookup_variable(expand->var_name, env));
                // var = ft_strjoin(" ", var);

            }

            
            // Only set var_value if the variable exists
            if (var != NULL)
                expand->var_value = var;
                
            free(expand->var_name);
            expand->var_name = NULL;
        }

        if (expand->var_value)
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
        }
        // We're not outputting anything for non-existent variables
        // And we're ensuring they are completely removed
        
        return (1);
    }
    return (0);
}

void  process_string_hp(t_exp_helper *expand)
{
    if (expand->expanded)
    {
	    expand->expanded[expand->j] = '\0';
        char *trim = ft_strtrim(expand->expanded, " ");
        if (!trim)
        {
            expand->expanded = NULL;
            return;
        }
        free(expand->expanded);
        expand->expanded = trim;
    }
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
    process_string_hp(expand);
}

int pls_conter_helper(char *str, int *pls_count, int *j, int *i)
{

    if (str[(*i)] == '+' && str[(*i) + 1] == '=' && ((*pls_count) == 0 || (*pls_count) == 1) && strchr(str, '\'') == NULL && strchr(str, '\"') == NULL)
    {
        if ((*i) == 0 || (!isalpha(str[0]) && str[0] != '_'))
            return 0; // Invalid key name
        while ((*j) < (*i))
        {
            if (!isalnum(str[(*j)]) && str[(*j)] != '_')
                return 0; // Invalid character in key name
            (*j)++;
        }
        return 1; // Valid += assignment
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
    while (str[i])
    {
        if (str[i] == '+')
            pls_count++;
        i++;
    }
    i = 0;
    while (str[i] && str[i] != '+')
        i++;
    return(pls_conter_helper(str, &pls_count, &j, &i));
}


// int pls_conter(char *str)
// {
//     int i;
//     int pls_count;
//     int j;
    
//     i = 0;
//     pls_count = 0;
//     j = 1;
//     while (str[i])
//     {
//         if (str[i] == '+')
//             pls_count++;
//         i++;
//     }
//     i = 0;
//     while (str[i] && str[i] != '+')
//         i++;
//     if (str[i] == '+' && str[i + 1] == '=' && (pls_count == 0 || pls_count == 1) && strchr(str, '\'') == NULL && strchr(str, '\"') == NULL)
//     {
//         if (i == 0 || (!isalpha(str[0]) && str[0] != '_'))
//             return 0; // Invalid key name
//         while (j < i)
//         {
//             if (!isalnum(str[j]) && str[j] != '_')
//                 return 0; // Invalid character in key name
//             j++;
//         }
//         return 1; // Valid += assignment
//     }
//     return 0;
// }

void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
    t_cmd *current;
    t_exp_helper *expand;
    t_redir *redir;
    int i;
    int should_split = 0;
    int pls_case = 0;
    int had_empty_var = 0;

    expand = malloc(sizeof(t_exp_helper));
    if (!expand)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        exit(1);
    }
    expand->buffer_size = 0;
    expand->expanded = NULL;
    expand->var_name = NULL;  // Add this
    expand->var_value = NULL; // Add this
    expand->had_removed_var = 0;


    current = cmd_list;
    while (current)
    {
        should_split = 0;
        i = 0;
        while (current->args && current->args[i])
        {
           if (pls_conter(current->args[i]) == 1)
                pls_case = 1;
          if (((current->args[i][0] == '\'' && current->args[i][strlen(current->args[i]) - 1] == '\'') ||
            (current->args[i][0] == '"' && current->args[i][strlen(current->args[i]) - 1] == '"')) && current->args[i + 1] != NULL)
                should_split = 0;
        else
        {
            if (strchr(current->args[i], '$'))
            {
                should_split = 1;
            }
            else 
            {
                should_split = 1;  // For other commands or non-export args
            }
        }

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
                // Also remove from args_befor_quotes_remover if it exists
                if (current->args_befor_quotes_remover && current->args_befor_quotes_remover[i])
                {
                    free(current->args_befor_quotes_remover[i]);
                    
                    // Shift all following arguments forward
                    int j = i;
                    while (current->args_befor_quotes_remover[j + 1])
                    {
                        current->args_befor_quotes_remover[j] = current->args_befor_quotes_remover[j + 1];
                        j++;
                    }
                    current->args_befor_quotes_remover[j] = NULL;
                }
                
                // Shift all following arguments forward in args
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
                process_string(redir->file, expand, env, exit_status, cmd_list->pipe_out);
                free(redir->file);
                redir->file = expand->expanded;
                expand->expanded = NULL;
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
        {
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
}
