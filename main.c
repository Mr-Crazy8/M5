/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:07:21 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/15 17:41:23 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int global_sig = 0;
int is_builtin(char **args)
{
	if(!args || !*args)
		return (1);
	if (strcmp(args[0], "cd") == 0)
		return (0);
	else if (strcmp(args[0], "echo") == 0)
		return (0);
	else if (strcmp(args[0], "unset") == 0)
		return (0);
	else if (strcmp(args[0], "export") == 0)
		return (0);
		else if (strcmp(args[0], "pwd") == 0)
			return (0);
		else if (strcmp(args[0], "env") == 0)
			return (0);
		else if (strcmp(args[0], "exit") == 0)
			return (0);
		else
			return (1);
}

void excute_builting(t_cmd **command, t_env *env_list, char *env[])
{
	t_cmd	*cmd;
	int		status;

	cmd = *command;
	status = 0;

	if (strncmp("export", cmd->args[0], 6) == 0 && strlen(cmd->args[0]) == 6)
		status = ft_export(cmd->args, &env_list);
	else if (strncmp("env", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
		status = ft_env(*command, env_list);
	else if (strncmp("exit", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
	{
		status = ft_exit(cmd->args, cmd->data);
		// free_cmd_list(cmd);
	}
	else if (strncmp("unset", cmd->args[0], 5) == 0 && strlen(cmd->args[0]) == 5)
	{
		status = ft_unset(&env_list, cmd->args + 1);
	}
	else if (strncmp("echo", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
		status = echo(cmd->args);
	else if (strncmp("pwd", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
	{
		status = pwd(cmd->data);
	}
	else if (strncmp("cd", cmd->args[0], 2) == 0 && strlen(cmd->args[0]) == 2)
		status = ft_cd(cmd->args, &env_list, cmd->data);
	cmd->data.exit_status = get_or_set(SET, status);
}
void execute_single_command(t_cmd *cmd, t_env *list_env, char *env[])
{
	    // signal(SIGINT, SIG_IGN);
        // signal(SIGQUIT, SIG_IGN);
		if(!is_builtin(cmd->args))
		{
			if (list_env) 
			{
            cmd->data.new_pwd = get_value_env("PWD", &list_env);
			}
		else 
		{
            cmd->data.new_pwd = NULL;
        }
			excute_builting(&cmd, list_env, env);
			get_or_set(SET, cmd->data.exit_status);
			return ;
		}
		else
		{
			ft_excute_commands(cmd, &list_env);
			// if(global_sig == 1)
			// 	get_or_set(SET, 130);
			get_or_set(SET, cmd->data.exit_status);
		}
}



void handel_signal(int sig)
{
	if (sig == SIGINT)
	{
		global_sig = sig;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// else if (sig == SIGQUIT)
	// {
	// 	global_sig = 131;
	// 	ft_putstr_fd("Quit: 3\n", 2); // like bash does
	// 	rl_on_new_line();
	// 	rl_replace_line("", 0);
	// 	rl_redisplay();
	// }
	// else if (sig == SIGQUIT)
	// {
	// 	puts("kkk");
	// 	global_sig = 0;
	// 	rl_redisplay();
	// }
}
void check_sig(t_cmd *cmd)
{
	if(global_sig != 0)
	{
		if (global_sig == 2)
			global_sig = 1;
		cmd->data.exit_status = get_or_set(SET, global_sig);
		// global_sig = 0;
	}
}

// void check_line(t_cmd **command, t_env *env_list, char *env[])
// {
// 	 t_cmd	*cmd;
// 	  cmd = *command;
// 	  int fd_input ;
// 	  int fd_output ;

// 	// signal(SIGINT, handel_signal);
// 	// signal(SIGQUIT, SIG_IGN);
//     fd_input = dup(0);
// 	fd_output = dup(1);
// 	// check_sig(cmd);
// 	check_here_doc(*command, env_list);
// 	if (cmd->pipe_out)
// 	{
// 		dup2(fd_input, 0);
// 		dup2(fd_output, 1);
// 		close(fd_input);
// 		close(fd_output);
// 		ft_excute_mult_pipe(cmd, env_list, env);
// 		// get_or_set(SET, cmd->data.exit_status);
// 		return ;
// 	}
// 	else
// 	{
// 		if(cmd->redirs)
// 		{
// 			if(cmd->redirs->fd == -1)
// 			{
// 					dup2(fd_input, 0);
// 					dup2(fd_output, 1);
// 					close(fd_input);
// 					close(fd_output);
// 					cmd->data.exit_status = get_or_set(SET, 1);
// 					return ;
// 			}
// 			ft_redircte(cmd->redirs, env_list, *command);
// 			execute_single_command(cmd, env_list, env);
// 		}
// 		else
// 			execute_single_command(cmd, env_list, env);
// 	}
// 	// check_sig(cmd);
// 	dup2(fd_input, 0);
// 	dup2(fd_output, 1);
// 	close(fd_input);
// 	close(fd_output);
// }

void check_line(t_cmd **command, t_env *env_list, char *env[])
{
    t_cmd    *cmd;
    cmd = *command;
    int fd_input ;
    int fd_output ;

    // if ((*command)->redirs != NULL)
    fd_input = dup(0);
    fd_output = dup(1);
        check_here_doc(*command, env_list);
    if (cmd->pipe_out)
    {
        ft_excute_mult_pipe(cmd, env_list, env);
        get_or_set(SET, cmd->data.exit_status);
        dup2(fd_input, 0);
        dup2(fd_output, 1);
        close(fd_input);
        close(fd_output);
        return ;
    }
    else
    {
        if(cmd->redirs)
        {
            if(cmd->redirs->fd[0] == -1)
            {
                    dup2(fd_input, 0);
                    dup2(fd_output, 1);
                    close(fd_input);
                    close(fd_output);
                    cmd->data.exit_status = get_or_set(SET, 1);
                    return ;
            }
            ft_redircte(cmd->redirs, env_list, *command);
            execute_single_command(cmd, env_list, env);
    dup2(fd_input, 0);
    dup2(fd_output, 1);
    close(fd_input);
    close(fd_output);
        }
        else
            execute_single_command(cmd, env_list, env);
    }
    dup2(fd_input, 0);
    dup2(fd_output, 1);
    close(fd_input);
    close(fd_output);
}

void add_one_shlvl(t_env *env)
{
    t_env *tmp = env;
    int shl_vl = 0;
    int found = 0;

    if (!env) 
        return;
    while (tmp) 
    {
        if (tmp->key && strcmp(tmp->key, "SHLVL") == 0)
        {
            found = 1;  // Mark that we found SHLVL
            if (tmp->value && tmp->value[0] != '\0')
            {
				// printf("SHLVL BEFOR : %s\n", tmp->value);
                shl_vl = atoi(tmp->value);
                free(tmp->value);
                tmp->value = NULL;
            }
            
            shl_vl++; 
            tmp->value = ft_itoa(shl_vl);
			// printf("SHLVL AFTER : %s\n", tmp->value);
            if (!tmp->value) 
                tmp->value = strdup("1");
            break;
        }
        tmp = tmp->next;
    }
    if (!found && env)
    {
        t_env *new_node = malloc(sizeof(t_env));
        if (!new_node)
            return;
            
        new_node->key = strdup("SHELVL");
        new_node->value = strdup("1");
        new_node->is_not_active = 0;
        new_node->next = NULL;
        tmp = env;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
}

int max_heredoc_checker(t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	t_redir *tp = NULL;
	int count = 0;
	while (tmp)
	{
		tp = tmp->redirs;
		while (tp)
		{
			if (tp->type == 3)
				count++;
			tp = tp->next;
		}
		tmp = tmp->next;
	}
	if (count > 16)
		{
			write(2, "minishell:  maximum here-document count exceeded\n", 50);
			get_or_set(SET, 2);
			return 1;
		}
	return 0;
}



int heredoc_count(t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	t_redir *tp = NULL;
	int count = 0;
	while (tmp)
	{
		tp = tmp->redirs;
		while (tp)
		{
			if (tp->type == 3)
				count++;
			tp = tp->next;
		}
		tmp = tmp->next;
	}

	return count;
}
void check_here_doc(t_cmd *cmd, t_env *env)
{
	 /* 0:<, 1:>, 2:>>, 3:<< */
	t_cmd *tmp;
	t_redir *tmp_redir;
	tmp_redir = NULL;
	tmp = cmd;
	if (max_heredoc_checker(cmd) == 1)
	{
		get_or_set(SET, 2);
		exit(2);
	}
	while (tmp)
	{
		tmp_redir = tmp->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == 3)
				{
					heredoc(tmp_redir->file, env, 0, tmp_redir->orig_token, tmp_redir->fd[0]);
				}
			tmp_redir = tmp_redir->next;
		}
		
		tmp = tmp->next;
	}	
}


char *chenger_back(char *str)
{
    int i = 0;

    while (str && str[i])
    {
        if (str[i] == 3)
            str[i] = '\'';
        else if (str[i] == 4)
            str[i] = '\"';
        i++;
    }
    return str;
}

void change_back_cmd(t_cmd *cmd)
{
	t_cmd *tmp;
	int i = 0;
	tmp = cmd;

	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			tmp->args[i] = chenger_back(tmp->args[i]);
			i++;
		}
		tmp->cmd = chenger_back(tmp->cmd);
		i = 0;
		t_redir *tp = tmp->redirs;
		while (tp)
		{
			tp->file = chenger_back(tp->file);
			tp = tp->next;
		}
		tmp = tmp->next;
	}
}
// void ff()
// {
// 	system("leaks minishell");
// }


char *change_space(char *str)
{
	int i = 0;
	int quote_state = 0;
	
	while(str && str[i])
	{
		if (str[i] == '\'')
		{
			if (quote_state == 0)
				quote_state = 1;
			else if (quote_state == 1)
				quote_state = 0;
		}
		else if (str[i] == '"')
		{
			if (quote_state == 0)
				quote_state = 2;
			else if (quote_state == 2)
				quote_state = 0;
		}
		if (str[i] >= 9 && str[i] <=13 && quote_state == 0)
			str[i] = ' ';
		i++;
	}
	return str;
}


// char *split_helper(char *str, char *befor, int exp)
// {
// 	int i = 1;
// 	int lent;
// 	char **split;
// 	char *key;
// 	char *new_value = NULL;
// 	char *join1;
// 	char *join3;
// 	char *join4 = NULL;
// 	char *add = NULL;
// 	char *join2;
// 	if (exp == 1)
// 	{
// 		 if (strchr(str, '=') != NULL)
// 		 {
// 			split = ft_split(str, '=');
// 			if (split != NULL)
//             {
// 				lent = ft_lint(split);
// 				key = ft_strtrim(split[0], "+");
// 				if ((((strchr(key, '\'') == NULL && strchr(key, '\"') == NULL) && strchr(key, '$') == NULL)))
// 				 {
// 					while(split && split[i])
// 					{
// 						if (split && split[i] && split[i][0] == '$')
// 						{
// 							join1 = ft_strjoin("\"", split[1]);
//                         	join2 = ft_strjoin(join1, "\"");
// 							if (i < lent)
// 								add = ft_strjoin(join2, "=");
// 							else
// 								add = NULL;
// 							new_value = ft_strjoin(new_value, add);
//                         	free(join1);
// 							free(join2);
// 							if (add != NULL)
// 								free(add);
							
// 						}
// 						i++;
// 					}
// 					join3 = ft_strjoin(new_value, "=");
//                 	join4 = ft_strjoin(join3, join2);
// 					free(join2);
//                     free(join3);
					
// 				 }
// 				else
//                     join4 = ft_strdup(str); 
// 			}
//             join4 = ft_strdup(str);
			
// 		 }
// 	}
// 	return join4;	
// }
//--------
// void free_extract_result(char **split)
// {
//     if (!split)
//         return;
    
//     if (split[0])
//         free(split[0]);
//     if (split[1])
//         free(split[1]);
//     free(split);
// }


// char **extract(char *str)
// {
// 	int i = 0;
// 	char *key;
// 	char *value;
// 	char **split = malloc(sizeof(char *) * 3);
	
// 	if (!split)
//         return NULL;
// 	while (str && str[i] && str[i] != '=')
// 		i++;
// 	key = ft_substr(str, 0, i);
// 	if (!key) 
// 	{
//         free(split);
//         return NULL;
//     }
// 	printf("key ======= [%s]\n", key);
// 	value = ft_substr(str, i+1, ft_strlen(str) - (i+1));
// 	if (!value) 
// 	{
//         free(key);
//         free(split);
//         return NULL;
//     }
// 	split[0] = key;
// 	split[1] = value;
// 	split[2] = NULL;
	
// 	return split;
// }

// char *plus_checker(char *str)
// {
// 	int i = 0;

// 	 if (!str)
//         return str;

		
// 	while (str[i])
// 		i++;
//     if (i > 0 && str[i - 1] == '+')
//     {
//         str[i - 1] = '\0';
//     }
// 	printf("_______{%s}_______\n", str);
// 	return str;
// }
// char *split_helper(char *str, char *befor, int exp)
// {
// 	int i = 1;
// 	int lent;
// 	char **split;
// 	char *key;
// 	char *new_value = NULL;
// 	char *join1 = NULL;
// 	char *join3 = NULL;
// 	char *join4 = NULL;
// 	char *add = NULL;
// 	char *join2 = NULL;
// 	char *new_key = NULL;
// 	if (exp == 1)
// 	{
// 		 if (strchr(str, '=') != NULL)
// 		 {
// 			split = extract(str);
// 			if (split != NULL)
//             {
// 				printf("split[0] ==========>  [%s]\n", split[0]);
// 				printf("split[1] ==========>  [%s]\n", split[1]);
// 				new_key = plus_checker(ft_strdup(split[0]));
				
// 				lent = ft_lint(split);
// 				printf("new_key ==========>  [%s]\n",new_key);
// 				if ((((strchr(split[0], '\'') == NULL && strchr(split[0], '\"') == NULL) && strchr(split[0], '$') == NULL)) && is_valid_key(new_key) == 0)
// 				 {
// 					if (split && split[1] && strchr(split[1], '$') != NULL)
// 					{
// 						join1 = ft_strjoin("\"", split[1]);
// 						if (!join1) 
// 						{
//                         free(new_key);
//                         free_extract_result(split);
						
//                         return NULL;
//                     	}
// 						join2 = ft_strjoin(join1, "\"");
// 						if (!join2) 
// 						{
//                         free(join1);
//                         free(new_key);
//                         free_extract_result(split);
//                         return NULL;
//                        }
// 						join3 = ft_strjoin("=", join2);
// 						 if (!join3) 
// 						 {
//                         free(join1);
//                         free(join2);
//                         free(new_key);
//                         free_extract_result(split);
//                         return NULL;
//                         }
//                 		join4 = ft_strjoin(split[0], join3);
// 						free(join1);
// 						free(join2);
//                     	free(join3);
// 					}
// 					else
// 						{
// 							join3 = ft_strjoin(split[0], "=");
// 							 if (!join3) 
// 							 {
//                         		free(new_key);
//                         		free_extract_result(split);
//                         		return NULL;
//                     		}
// 							join4 = ft_strjoin(join3, split[1]);
// 							free(join3);
// 						}
					
// 				 }
// 				else
//                     join4 = ft_strdup(str);
// 				free(new_key); 
// 				free_extract_result(split);
// 			}
//            	else
//                 join4 = ft_strdup(str); 
			
// 		 }
// 		 else
//                 join4 = ft_strdup(str);
// 	}
// 	return join4;	
// }
// void split_stoper(t_cmd *cmd)
// {
//     t_cmd *tmp;
//     tmp = cmd;
//     char *new_str;
//     int exp = 0;
    
//     while(tmp)
//     {
//         int i = 0;
//         exp = 0; 
        
//         while (tmp->args && tmp->args[i])
//         {
//             if (i == 0 && strcmp(tmp->args[i], "export") == 0) 
//                 exp = 1;
//             if (tmp->args_befor_quotes_remover && tmp->args_befor_quotes_remover[i])
//             {
//                 new_str = split_helper(tmp->args[i], tmp->args_befor_quotes_remover[i], exp);
//                 if (new_str != NULL)
//                 {
//                     free(tmp->args[i]);
//                     tmp->args[i] = new_str;
//                 }
//             }
//             i++;
//         }
//         tmp = tmp->next;
//     }
// }-------

void free_extract_result(char **split)
{
    if (!split)
        return;
    
    if (split[0])
        free(split[0]);
    if (split[1])
        free(split[1]);
    free(split);
}


char **extract(char *str)
{ 
    int i = 0;
    char *key;
    char *value;
    char **split = malloc(sizeof(char *) * 3);
    
    if (!split)
        return NULL;
    while (str && str[i] && str[i] != '=')
        i++;
    key = ft_substr(str, 0, i);
    if (!key) 
    {
        free(split);
        return NULL;
    }
    value = ft_substr(str, i+1, ft_strlen(str) - (i+1));
    if (!value) 
    {
        free(key);
        free(split);
        return NULL;
    }
    split[0] = key;
    split[1] = value;
    split[2] = NULL;
    
    return split;
}

char *plus_checker(char *str)
{
    int i = 0;
    
    if (!str)
        return str;
        
    while (str[i])
        i++;
        
    if (i > 0 && str[i - 1] == '+')
        str[i - 1] = '\0';
    return str;
}

char *split_helper(char *str, char *befor, int exp)
{
    char **split;
    char *join1 = NULL;
    char *join3 = NULL;
    char *join4 = NULL;
    char *join2 = NULL;
    char *new_key = NULL;
    
    if (exp != 1)
        return NULL;
        
    if (strchr(str, '=') != NULL)
    {
        split = extract(str);
        if (split != NULL)
        {
            new_key = ft_strdup(split[0]);
            if (!new_key)
            {
                free_extract_result(split);
                return NULL;
            }
            
            new_key = plus_checker(new_key);
            if ((((strchr(split[0], '\'') == NULL && strchr(split[0], '\"') == NULL) && 
                 strchr(split[0], '$') == NULL)) && is_valid_key(new_key) == 0)
            {
                if (split && split[1] && strchr(split[1], '$') != NULL)
                {
                    join1 = ft_strjoin("\"", split[1]);
                    if (!join1) 
                    {
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join2 = ft_strjoin(join1, "\"");
                    if (!join2) 
                    {
                        free(join1);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join3 = ft_strjoin("=", join2);
                    if (!join3) 
                    {
                        free(join1);
                        free(join2);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join4 = ft_strjoin(split[0], join3);
                    if (!join4)
                    {
                        free(join1);
                        free(join2);
                        free(join3);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    free(join1);
                    free(join2);
                    free(join3);
                }
                else
                {
                    join3 = ft_strjoin(split[0], "=");
                    if (!join3) 
                    {
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join4 = ft_strjoin(join3, split[1]);
                    if (!join4)
                    {
                        free(join3);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    free(join3);
                }
            }
            else
            {
                join4 = ft_strdup(str);
                if (!join4)
                {
                    free(new_key);
                    free_extract_result(split);
                    return NULL;
                }
            }
            free(new_key); 
            free_extract_result(split);
        }
        else
        {
            join4 = ft_strdup(str);
            if (!join4)
                return NULL;
        }
    }
    else
    {
        join4 = ft_strdup(str);
        if (!join4)
            return NULL;
    }
    return join4;    
}

void split_stoper(t_cmd *cmd)
{
    t_cmd *tmp;
    char *new_str;
    int exp = 0;
    
    if (!cmd)
        return;
        
    tmp = cmd;
    while(tmp)
    {
        int i = 0;
        exp = 0; 
        
        while (tmp->args && tmp->args[i])
        {
            if (i == 0 && tmp->args[i] && strcmp(tmp->args[i], "export") == 0) 
                exp = 1;
            if (tmp->args_befor_quotes_remover && tmp->args_befor_quotes_remover[i])
            {
                new_str = split_helper(tmp->args[i], tmp->args_befor_quotes_remover[i], exp);
                if (new_str != NULL)
                {
                    free(tmp->args[i]);
                    tmp->args[i] = new_str;
                }
            }
            i++;
        }
        tmp = tmp->next;
    }
}
int main(int argc, char *argv[], char *env[])
{
	t_token *token_list;
	t_env *env_struct = NULL;
	char *input;
	t_cmd *cmd = NULL;
	// atexit(ff);
	int exit_status = 0;
	// cmd->data.exit_status = 0;
	char *preprocessed_input;
		struct termios infos;
	(void)argc;
	(void)argv;
	if(!isatty(1) || !isatty(0))     /// must stay
	{
		exit(1);
	}
	env_struct = env_maker(env, &env_struct);   /// must stay
	if(!env_struct)									/// must stay
		env_null(&env_struct); 							/// must stay
	add_one_shlvl(env_struct);      					/// must stay
	token_list = NULL;
	tcgetattr(1, &infos);
	infos.c_lflag &= ~(ECHOCTL);
	tcsetattr(1, TCSANOW, &infos);
	while (1)
	{
		
		signal(SIGINT, handel_signal);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell $> ");
		// input = ft_strdup("export $l=$a m+=$a n\"=$a\"");
		if (!input)
		{
			printf("exit\n");
			// free_env_struct(env_struct);
			// if (cmd) 
			// 	free_cmd_list(cmd);
    		// if (token_list) 
			// 	free_token_list(token_list);
			break ;
		}
		add_history(input);
		if (check_quotes(input))         /// must stay
		{
			exit_status = get_or_set(SET, 258); 
			free(input);
			continue;
		}
		preprocessed_input = preprocess_command(input);     /// must stay 
			free(input);
         if (!preprocessed_input)
		 {
			free(input);
            continue;
		 }
		 char *new_input = change_space(preprocessed_input);     /// must stay
		token_list = tokin_list_maker(new_input);  /// must stay
		
		free(preprocessed_input);
		if (token_list && !error_pipi(token_list)  && !check_syntax_errors(token_list))    /// must stay
		{
			cmd = parser(token_list);/// must stay
			split_stoper(cmd);/// must stay
			free_token_list(token_list);/// must stay
			expand_handle(cmd, env_struct, get_or_set(GET, 0));/// must stay
			ambiguous_finder(cmd);/// must stay
			
			process_quotes_for_cmd(cmd, 1);/// must stay
			change_back_cmd(cmd);/// must stay
			file_opener(cmd, env_struct);/// must stay
			print_ambiguous_redir_errors(cmd);/// must stay
			print_cmd(cmd);
			check_line(&cmd, env_struct, env);/// must stay
			//expand_handle(cmd, env_struct, cmd->data.exit_status);
			
			free_cmd_list(cmd);/// must stay
			global_sig = 0;/// must stay
			
		}
		else if (error_pipi(token_list)  || check_syntax_errors(token_list))/// must stay
			exit_status = get_or_set(SET, 258); /// must stay
		else if (token_list)/// must stay
		{
			free_token_list(token_list);/// must stay
		}

		// 	infos.c_lflag &= ~(ECHOCTL);
		// tcsetattr(1, TCSANOW, &infos);
	}
	
	free_env_struct(env_struct);
	return 0;
}


