/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lll.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:22:58 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/12 17:53:40 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void ft(char **args, t_env **list) // pipe moust be check and access;;;;*
{
    char **split_path;
    char **helper;
    char *path;
    int i= 0;
    char *tmp;

    helper = convert_env_list(list);
    path = check_path(list);
    if(!args || !args[0])
        return ;
    if(ft_strchr(args[0], '/'))
    {
        if(!(access(args[0], X_OK) == -1))
        {
            execve(args[0], args, helper);
            exit (1); //ila failat execve
        }
        write(1, "command not found\n", 18);
    }
    else
    {
        split_path = ft_split(path, ':');
        if(!split_path)
            return ;
        while(split_path[i])
        {
            tmp = ft_strjoin(split_path[i], "/");
            char *str = ft_strjoin(tmp, args[0]);
            free(tmp);
            if (!helper)
            {
                i++;
                continue;
            }
            if(!(access(str, X_OK) == -1))
            {
                execve(str, args, helper);
                exit (1);
            }
            free(str);
            i++;
        }
        i = 0;
        write(2, "command not found\n", 18);
        ft_free_split(split_path);
        exit(127);
    }        
}
void    pipe_all(t_cmd *head)
{
    t_cmd *cmd = head;

    while (cmd)
    {
        
        if (cmd->pipe_out)
        {
            pipe(cmd->fd_pipe); // creates cmd->fd_pipe[0] (read) and [1] (write)
        }
        cmd = cmd->next;
    }
}
char **convert_env_list(t_env **list)
{
    t_env * tmp;
    char **helper = NULL;
    char *str;
    char *temp;
    int  i = 0;
    tmp = *list;
    int len = ft_list_size(*list);

    helper = malloc(sizeof(char *) * (len + 1));
    while(tmp)
    {
        str = ft_strdup(tmp->key);
        temp = ft_strjoin(str, "=");
        free(str);
        str = ft_strjoin(temp, tmp->value);
        free(temp);
        helper[i] = ft_strdup(str);
        free(str);
        tmp = tmp->next;
        i++;
    }
    helper[i] = NULL;
    return (helper);
}
void close_all_pipes(t_cmd *cmd)
{
    t_cmd *tmp = cmd;
    
    while (tmp)
    {
        if (tmp->pipe_out)
        {
            close(tmp->fd_pipe[0]);
            close(tmp->fd_pipe[1]);
        }
        tmp = tmp->next;
    }
}

void check_close_red(t_cmd *cmd, t_cmd *prev, t_env *env)
{
        if (prev && prev->pipe_out)
        {
            close(prev->fd_pipe[0]);
            close(prev->fd_pipe[1]);
        }
        if (cmd->pipe_out)
        {
            close(cmd->fd_pipe[0]);
            close(cmd->fd_pipe[1]);
        }
        if (!isatty(cmd->redirs->fd[0])) //check if this function is working;
            ft_redircte(cmd->redirs, env, cmd);
}

void handl_si(int sig)
{
    ft_putstr_fd("Quit: 3\n", 1);
    // cmd->data.exit_status = get_or_set(SET, 131);
    rl_redisplay();
}

void ft_free_redrect(t_redir *redir)
{
    t_redir *tmp = redir;
    if(!tmp)
        return ;
    while (tmp && tmp->fd)
    {
        close(tmp->fd[0]);
        tmp = tmp->next;
    }

}
void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[])
{
    pid_t pid;
    t_cmd *prev = NULL;
    t_cmd *head = NULL;
    t_cmd *tmp = cmd;
    int last_pid = 0;
    int status = 0;
    int last_status = 0;

    head = cmd;
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    pipe_all(cmd);
    while (cmd)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            close_all_pipes(head);
            return;
        }
        else if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, handl_si);
            if (prev && prev->pipe_out)
            {
                dup2(prev->fd_pipe[0], 0);
            }
            if (cmd->pipe_out)
                dup2(cmd->fd_pipe[1], 1);
            if(cmd->redirs != NULL)
            {
                check_close_red(cmd, prev, list_env);
            }
            close_all_pipes(head);
            if(cmd->redirs == NULL || cmd->redirs->fd[0] != -1)
                execute_single_command(cmd, list_env, env);
            last_status = cmd->data.exit_status;
            exit(cmd->data.exit_status);
        }
        if(cmd->redirs != NULL)
            ft_free_redrect(cmd->redirs);
        if(cmd->next == NULL)
            last_pid = pid;

        // Parent closes previous pipe ends
        if (prev && prev->pipe_out)
        {
            close(prev->fd_pipe[0]);
            close(prev->fd_pipe[1]);
        }
        prev = cmd;
        cmd = cmd->next;
    }
    close_all_pipes(head);
    	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				if (sig == SIGQUIT)
					ft_putstr_fd("Quit: 3\n", 1);
				else if (sig == SIGINT)
					ft_putstr_fd("\n", 1);
				last_status = 128 + sig;
			}
            head->data.exit_status = get_or_set(SET, last_status);
		}
	}
    // while (wait(&status) > 0)
    // {
    //     if (WIFEXITED(status))
    //         last_status = WEXITSTATUS(status);
    //     else if (WIFSIGNALED(status))
    //     {
    //         int sig = WTERMSIG(status);
    //         last_status = 128 + sig;
    //     }
    // }
    // Set the exit status to the last command's status
    // close_all_pipes(head);
    // pid_t wpid;
    // while ((pid = waitpid(pid, &status, 0)) > 0)
    // {
    //     if (WIFEXITED(status))
    //     {
    //         last_status = WEXITSTATUS(status);
    //     }
    // }
    get_or_set(SET, last_status);
    
    // // Set the exit status to the last command's status
    // get_or_set(SET, last_status);

    // while (wait(NULL) > 0)
    //     ;
}


// void ft(char **args, t_env **list) // pipe moust be check and access;;;;*
// {
//     char **split_path;
//     char **helper;
//     char *path;
//     int i= 0;
//     char *tmp;

//     helper = convert_env_list(list);
//     path = check_path(list);
//     if(!args || !args[0])
//         return ;
//     if(ft_strchr(args[0], '/'))
//     {
//         if(!(access(args[0], X_OK) == -1))
//         {
//             execve(args[0], args, helper);
//             exit (1); //ila failat execve
//         }
//         write(1, "command not found\n", 18);
//     }
//     else
//     {
//         split_path = ft_split(path, ':');
//         if(!split_path)
//             return ;
//         while(split_path[i])
//         {
//             tmp = ft_strjoin(split_path[i], "/");
//             char *str = ft_strjoin(tmp, args[0]);
//             free(tmp);
//             if (!helper)
//             {
//                 i++;
//                 continue;
//             }
//             if(!(access(str, X_OK) == -1))
//             {
//                 execve(str, args, helper);
//                 exit (1);
//             }
//             free(str);
//             i++;
//         }
//         i = 0;
//         write(2, "command not found\n", 18);
//         ft_free_split(split_path);
//         exit(127);
//     }        
// }
// void    pipe_all(t_cmd *head)
// {
//     t_cmd *cmd = head;

//     while (cmd)
//     {
        
//         if (cmd->pipe_out)
//         {
//             pipe(cmd->fd_pipe); // creates cmd->fd_pipe[0] (read) and [1] (write)
//         }
//         cmd = cmd->next;
//     }
// }
// char **convert_env_list(t_env **list)
// {
//     t_env * tmp;
//     char **helper = NULL;
//     char *str;
//     char *temp;
//     int  i = 0;
//     tmp = *list;
//     int len = ft_list_size(*list);

//     helper = malloc(sizeof(char *) * (len + 1));
//     while(tmp)
//     {
//         str = ft_strdup(tmp->key);
//         temp = ft_strjoin(str, "=");
//         free(str);
//         str = ft_strjoin(temp, tmp->value);
//         free(temp);
//         helper[i] = ft_strdup(str);
//         free(str);
//         tmp = tmp->next;
//         i++;
//     }
//     helper[i] = NULL;
//     return (helper);
// }
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

// void check_close_red(t_cmd *cmd, t_cmd *prev, t_env *env)
// {
//         if (prev && prev->pipe_out)
//         {
//             close(prev->fd_pipe[0]);
//             close(prev->fd_pipe[1]);
//         }
//         if (cmd->pipe_out)
//         {
//             close(cmd->fd_pipe[0]);
//             close(cmd->fd_pipe[1]);
//         }
//         if (!isatty(cmd->redirs->fd)) //check if this function is working;
//             ft_redircte(cmd->redirs, env, cmd);
// }

// void handl_si(int sig)
// {
//     ft_putstr_fd("Quit: 3\n", 1);
//     // cmd->data.exit_status = get_or_set(SET, 131);
//     rl_redisplay();
// }
// void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[])
// {
//     pid_t pid;
//     t_cmd *prev = NULL;
//     t_cmd *head = NULL;
//     int last_pid = 0;
//     int status = 0;
//     int last_status = 0;

//     head = cmd;
//     pipe_all(cmd);

//     while (cmd)
//     {
//         pid = fork();
//         if (pid < 0)
//         {
//             perror("fork");
//             close_all_pipes(head);
//             return;
//         }
//         else if (pid == 0)
//         {
//             signal(SIGINT, SIG_DFL);
//             signal(SIGQUIT, handl_si);
//             if (prev && prev->pipe_out)
//                 dup2(prev->fd_pipe[0], 0);
//             if (cmd->pipe_out)
//                 dup2(cmd->fd_pipe[1], 1);
//             if(cmd->redirs != NULL)
//                 check_close_red(cmd, prev, list_env);

//             close_all_pipes(head);
//             if(cmd->redirs == NULL || cmd->redirs->fd != -1)
//                 execute_single_command(cmd, list_env, env);
//             // printf("{{{{{%d\n", cmd->data.exit_status);
//             last_status = cmd->data.exit_status;
//             exit(cmd->data.exit_status);
//         }
//         if(cmd->next == NULL)
//             last_pid = pid;

//         // Parent closes previous pipe ends
//         if (prev && prev->pipe_out)
//         {
//             close(prev->fd_pipe[0]);
//             close(prev->fd_pipe[1]);
//         }
//         prev = cmd;
//         cmd = cmd->next;
//     }
//     close_all_pipes(head);
//     	while ((pid = wait(&status)) > 0)
// 	{
// 		if (pid == last_pid)
// 		{
// 			if (WIFEXITED(status))
// 				last_status = WEXITSTATUS(status);
// 			else if (WIFSIGNALED(status))
// 			{
// 				int sig = WTERMSIG(status);
// 				if (sig == SIGQUIT)
// 					ft_putstr_fd("Quit: 3\n", 1);
// 				else if (sig == SIGINT)
// 					ft_putstr_fd("\n", 1);
// 				last_status = 128 + sig;
// 			}
//             // printf("...%d\n", last_status);
// 		}
// 	}
//     // while (wait(&status) > 0)
//     // {
//     //     if (WIFEXITED(status))
//     //         last_status = WEXITSTATUS(status);
//     //     else if (WIFSIGNALED(status))
//     //     {
//     //         int sig = WTERMSIG(status);
//     //         last_status = 128 + sig;
//     //     }
//     // }
//     // Set the exit status to the last command's status
//     // close_all_pipes(head);
//     // pid_t wpid;
//     // while ((pid = waitpid(pid, &status, 0)) > 0)
//     // {
//     //     if (WIFEXITED(status))
//     //     {
//     //         last_status = WEXITSTATUS(status);
//     //     }
//     // }
//     get_or_set(SET, last_status);
    
//     // // Set the exit status to the last command's status
//     // get_or_set(SET, last_status);

//     // while (wait(NULL) > 0)
//     //     ;
// }
