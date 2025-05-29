// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/25 20:41:00 by ayoakouh          #+#    #+#             */
// /*   Updated: 2025/05/07 14:26:07 by ayoakouh         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */




// // Create pipes for all commands that need them
// void pipe_all(t_cmd *head)
// {
//     t_cmd *cmd = head;

//     while (cmd)
//     {
//         if (cmd->pipe_out)
//         {
//             if (pipe(cmd->fd_pipe) < 0) // Check for pipe creation errors
//             {
//                 perror("pipe error");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         cmd = cmd->next;
//     }
// }

// int list_size(t_cmd *begin_list)
// {
//     int size = 0;
    
//     while (begin_list)
//     {
//         begin_list = begin_list->next;
//         size += 1;
//     }
//     return (size);
// }

// // Close all pipes to prevent leaks
// void close_all_pipes(t_cmd *cmd)
// {
//     t_cmd *tmp = cmd;
    
//     while (tmp)
//     {
//         if (tmp->pipe_out)
//         {
//             close(tmp->fd_pipe[0]);
//             close(tmp->fd_pipe[1]);
//         }
//         tmp = tmp->next;
//     }
// }

// void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[])
// {
//     pid_t pid1;
//     t_cmd *prev = NULL;
//     t_cmd *head = cmd;
//     int fd_input, fd_output;
    
//     // Save original stdin/stdout
//     fd_input = dup(STDIN_FILENO);
//     fd_output = dup(STDOUT_FILENO);
    
//     if (fd_input < 0 || fd_output < 0)
//     {
//         perror("dup error");
//         return;
//     }
    
//     cmd->total_pipes = list_size(cmd) - 1;
//     pipe_all(cmd);
    
//     while (cmd)
//     {
//         pid1 = fork();
//         if (pid1 < 0)
//         {
//             perror("fork error");
//             close_all_pipes(head);
//             close(fd_input);
//             close(fd_output);
//             return;
//         }
//         else if (pid1 == 0)  // Child process
//         {
//             // Close all pipes not needed by this process
//             t_cmd *tmp = head;
//             while (tmp)
//             {
//                 // Close pipes that this command doesn't need
//                 if (tmp != cmd && tmp != prev)
//                 {
//                     if (tmp->pipe_out)
//                     {
//                         close(tmp->fd_pipe[0]);
//                         close(tmp->fd_pipe[1]);
//                     }
//                 }
//                 tmp = tmp->next;
//             }
            
//             // Handle redirections first - they take precedence
//             if (cmd->redirs != NULL)
//             {
//                 // Close any pipes that would be overridden by redirections
//                 if (prev && prev->pipe_out)
//                 {
//                     close(prev->fd_pipe[0]);
//                     close(prev->fd_pipe[1]);
//                 }
//                 if (cmd->pipe_out)
//                 {
//                     close(cmd->fd_pipe[0]);
//                     close(cmd->fd_pipe[1]);
//                 }
                
//                 ft_redircte(cmd->redirs);
//             }
//             else 
//             {
//                 // Setup input from previous pipe if needed
//                 if (prev != NULL && prev->pipe_out)
//                 {
//                     dup2(prev->fd_pipe[0], STDIN_FILENO);
//                     close(prev->fd_pipe[0]);
//                     close(prev->fd_pipe[1]);
//                 }
                
//                 // Setup output to next pipe if needed
//                 if (cmd->pipe_out)
//                 {
//                     dup2(cmd->fd_pipe[1], STDOUT_FILENO);
//                     close(cmd->fd_pipe[0]);
//                     close(cmd->fd_pipe[1]);
//                 }
//             }
            
//             ft_excute_commands(cmd, &list_env);
//             exit(127);
//         }
        
//         // Parent process: close pipes we don't need anymore
//         if (prev && prev->pipe_out)
//         {
//             close(prev->fd_pipe[0]);
//             close(prev->fd_pipe[1]);
//         }
        
//         prev = cmd;
//         cmd = cmd->next;
//     }
    
//     // Close any remaining pipes in parent
//     if (prev && prev->pipe_out)
//     {
//         close(prev->fd_pipe[0]);
//         close(prev->fd_pipe[1]);
//     }
    
//     // Wait for all child processes to finish
//     while (wait(NULL) > 0)
//         ;
    
//     // Restore original stdin/stdout
//     dup2(fd_input, STDIN_FILENO);
//     dup2(fd_output, STDOUT_FILENO);
//     close(fd_input);
//     close(fd_output);
// }
// // #include "minishell.h"

// // // int ft_check_rederction(t_redir *cmd)
// // // {
// // //     t_redir *tmp;

// // //     tmp = cmd;
// // //     // while(tmp)
// // //     // {
// // //         if(tmp->redirs != NULL)
// // //         {
// // //             ft_redircte(&cmd);
// // //             return (0);
// // //         }
// // //         // tmp = tmp->next;
// // //     // }
// // //     return (1);
// // // }
// // int is_builtin(char **args)
// // {
// // 	if (strcmp(args[0], "cd") == 0)
// // 		return (0);
// // 	else if (strcmp(args[0], "echo") == 0)
// // 		return (0);
// // 	else if (strcmp(args[0], "unset") == 0)
// // 		return (0);
// // 	else if (strcmp(args[0], "export") == 0)
// // 		return (0);
// // 		else if (strcmp(args[0], "pwd") == 0)
// // 			return (0);
// // 		else if (strcmp(args[0], "env") == 0)
// // 			return (0);
// // 		else if (strcmp(args[0], "exit") == 0)
// // 			return (0);
// // 		else
// // 			return (1);
// // }
// // // void minishell_cmd(char *env[])
// // // {
// // // 	pid_t pid = fork();
// // // 		if (pid == 0)
// // // 		{
// // // 			char *args[] = {"./minishell", NULL};
// // // 			execve("./minishell", args, env);
// // // 		}
// // // 		else
// // // 		{
// // // 			waitpid(pid, NULL, 0);
// // // 		}
// // // }
// // int get_or_set(int type, int status)
// // {
// // 	static int exit_status = 0; // hold the value even after function ends

// // 	if (type == SET) //SET
// // 		exit_status = status; // rje3 dkre hadik kifch dkhlha m3a exit_status;
// // 	return (exit_status); // always return the current value
// // }

// // void excute_builting(t_cmd **command, t_env *env_list, char *env[])
// // {
// // 	t_cmd	*cmd;
// // 	int		status;
// // 	t_env  *help;

// // 	cmd = *command;
// // 	status = 0;

// // 	if (strncmp("export", cmd->args[0], 6) == 0 && strlen(cmd->args[0]) == 6)
// // 		status = ft_export(cmd->args, &env_list);
// // 	else if (strncmp("env", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
// // 		status = ft_env(*command, env_list);
// // 	else if (strncmp("exit", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
// // 	{
// // 		ft_exit(cmd->args, cmd->data);
// // 		free_cmd_list(cmd);
// // 	}
// // 	else if (strncmp("unset", cmd->args[0], 5) == 0 && strlen(cmd->args[0]) == 5)
// // 	{ 
// // 		status = ft_unset(&env_list, cmd->args + 1);
// // 	}
// // 	else if (strncmp("echo", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
// // 		status = echo(cmd->args);
// // 	else if (strncmp("pwd", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
// // 		status = pwd();
// // 	else if (strncmp("cd", cmd->args[0], 2) == 0 && strlen(cmd->args[0]) == 2)
// // 		status = ft_cd(cmd->args, &env_list);
// // 	cmd->data.exit_status = get_or_set(SET, status);
// // 	printf("%d\n", cmd->data.exit_status);
// // }
// // void excute_siingle(t_cmd *cmd, t_env *list_env, char *env)
// // {
// // 	if(cmd->redirs != NULL)
// // 	{
// // 		ft_redircte(cmd->redirs);
// // 	}
// // 	if(!is_builtin(cmd->args))
// // 	{
// // 		dprintf(2, "hllo\n");
// // 		excute_builting(&cmd, list_env, env);
// // 	}
// // 	else
// // 	{
// // 		ft_excute_commands(cmd, &list_env);
// // 		printf(".....%d\n", cmd->data.exit_status);
// // 	}
// // }

// // void check_line(t_cmd **command, t_env *env, char *en[])
// // {
// // 	 t_cmd *cmd;
// // 	  cmd = *command;
// // 	  int fd_input ;
// // 	  int fd_output ;
// // 		fd_input = dup(0);
// // 		fd_output = dup(1);
// // 		if (cmd->pipe_out == 1)
// // 		{
// // 			ft_excute_mult_pipe(cmd, env, en);
// // 			return ;
// // 		}
// // 	else
// // 	{
// // 	//   while(cmd)
// // 	//   {
// // 			if(cmd->redirs != NULL)
// // 			{
// // 				ft_redircte(cmd->redirs);
// // 			}
// // 			if(!is_builtin(cmd->args))
// // 			{
// // 				excute_builting(&cmd, env, en);
// // 			}
// // 			else
// // 			{
// // 				ft_excute_commands(cmd, &env);
// // 				printf(".....%d\n", cmd->data.exit_status);
// // 			}
// // 			// cmd = cmd->next;
// // 		// }
// // 	}
// // 	dup2(fd_input, 0);
// // 	dup2(fd_output, 1);
// // 	close(fd_input);
// // 	close(fd_output);
// // }
// // // void ff()
// // // {
// // // 	system("lsof minishell");
// // // }
// // int main(int argc, char *argv[], char *env[])
// // {
// // 	// atexit(ff);
// // 	t_token *token_list;
// // 	t_env *env_struct = NULL;   //// add to the final main
// // 	int exit_status = 0;
// // 	char *input;
// // 	t_cmd *cmd;
	
// // 	(void)argc;
// // 	(void)argv;

// // 	env_struct = env_maker(env, &env_struct);
// // 	token_list = NULL;
// // 	while (1)
// // 	{
// // 		input = readline("minishell $> ");
// // 		// cmd = NULL;
// // 		if (!input)
// // 			break;
// // 		add_history(input);
// // 		if (check_quotes(input))
// // 		{
// // 			// Error message already printed by check_quotes
// // 			free(input);
// // 			continue;
// // 		}
// // 		token_list = tokin_list_maker(input);
// // 		if (token_list && !error_pipi(token_list)  && !check_syntax_errors(token_list))
// // 		{
// // 			// printf("--- TOKENS ---\n");
// // 			expand_handle(token_list, env_struct, exit_status);
// // 			//process_quotes_for_tokens(token_list, 1);
// // 			cmd = parser(token_list);
// // 			// ft_excute(cmd);
// // 			check_line(&cmd, env_struct, env);
// // 			//debug_print_cmd(cmd);
// // 			if (cmd == NULL) {
// // 				printf("Warning: Command list is empty after parsing!\n");
// // 			} else {
// // 				// process_quotes_for_cmd(cmd, 1);
// // 				// print_cmd(cmd);
// // 				// if (cmd)
// // 				// 	free_cmd_list(cmd);
// // 			}
// // 		}   
// // 		free_token_list(token_list);
// // 		free(input);
// // 	}
// // 	free_env_struct(env_struct);
// // 	return 0;
// // }