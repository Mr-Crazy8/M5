/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redrction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:59:21 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/04 20:25:09 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void intialize_struct(t_redir *list)
// {
//     t_redir *tmp = list;

//     tmp->type  = 0;
//     tmp->file = strdup("input.txt");
// }
// void ft_herdoc(t_redir *red, char *delimeter)
// {
//     t_redir *tmp;

//     tmp = red;
//     delimeter = tmp->file;
//     int fd = open("/tmp/.heredoc_tmp", O_CREAT | O_RDWR | O_TRUNC, 0664);

//     while(1)
//     {
//         char *line = readline("herdoc> ");
//         if(!ft_strcmp(delimeter, line))
//             break ;
//         write (fd, line, ft_strlen(line));
//         write (1, "\n", 1);
//         free(line);
//     }
//     // int infile = open("/tmp/.heredoc_tmp", O_RDONLY);

// }

void ft_redircte(t_redir *rederction, t_env *env, t_cmd *cmd)
{
    t_redir *tmp;

    tmp = rederction;

    while(tmp)
    {
        if(tmp->type == 0)
        {
            dup2(tmp->fd, 0);
            close(tmp->fd); 
        }
        else if (tmp->type == 1)
        {
            dup2(tmp->fd, 1);
            close(tmp->fd);
            
        }
        else if(tmp->type == 2 )
        {
            dup2(tmp->fd, 1);
            close (tmp->fd);
        }  
        else if (tmp->type == 3)
        {
          if (tmp->fd >= 0)
            {
                dup2(tmp->fd, 0);
                close(tmp->fd);  // Close after duplication
                    tmp->fd = -1;    // Mark as closed
            }
        }
        tmp = tmp->next;
    }
}

void free_cmd(t_cmd *cmd)
{
    t_redir *redir;
    t_redir *next_redir;
    
    if (cmd->cmd)
        free(cmd->cmd);
    
    if (cmd->args)
    {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
    
    redir = cmd->redirs;
    while (redir)
    {
        next_redir = redir->next;
        if (redir->file)
            free(redir->file);
        free(redir);
        redir = next_redir;
    }
    free(cmd);
}

// int main(void)
// {
//     // Test case 1: Simple ls command with input redirection
//     printf("=== Test 1: ls with input redirection ===\n");
    
//     // Create command: ls -la
//     char **args = malloc(3 * sizeof(char *));
//     args[0] = strdup("ls");
//     args[1] = strdup("-la");
//     args[2] = NULL;
    
//     t_cmd *cmd = new_cmd("ls", args);
    
//     // Add input redirection
//     t_redir *redir = new_redir(0, "input.txt");
//     cmd->redirs = redir;
    
//     // Create a test input file
//     int fd = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd != -1) {
//         write(fd, "Test input file content\n", 23);
//         close(fd);
//     }
    
//     // Execute the redirection test
//     printf("Command: %s\n", cmd->cmd);
//     printf("Arguments: %s %s\n", cmd->args[0], cmd->args[1]);
//     printf("Redirection type: %d, file: %s\n", cmd->redirs->type, cmd->redirs->fd_file);
    
//     // Instead of actually executing (which would replace this process),
//     // we'll just print what would happen
//     printf("This would redirect input from %s and execute %s %s\n\n", 
//            cmd->redirs->fd_file, cmd->args[0], cmd->args[1]);

//     // Test case 2: Create a pipeline
//     printf("=== Test 2: Command pipeline ===\n");
    
//     // First command: ls -l
//     char **args1 = malloc(3 * sizeof(char *));
//     args1[0] = strdup("ls");
//     args1[1] = strdup("-l");
//     args1[2] = NULL;
    
//     t_cmd *cmd1 = new_cmd("ls", args1);
//     cmd1->pipe_out = 1; // This command pipes to the next
    
//     // Second command: grep ".c"
//     char **args2 = malloc(3 * sizeof(char *));
//     args2[0] = strdup("grep");
//     args2[1] = strdup(".c");
//     args2[2] = NULL;
    
//     t_cmd *cmd2 = new_cmd("grep", args2);
//     cmd2->pipe_out = 0; // End of pipeline
    
//     // Link commands
//     cmd1->next = cmd2;
    
//     // Print pipeline info
//     t_cmd *temp = cmd1;
//     int cmd_num = 1;
    
//     while(temp) {
//         printf("Command %d: %s\n", cmd_num, temp->cmd);
//         printf("  Arguments: ");
//         for(int i = 0; temp->args[i]; i++) {
//             printf("%s ", temp->args[i]);
//         }
//         printf("\n");
//         printf("  Pipe out: %d\n", temp->pipe_out);
        
//         temp = temp->next;
//         cmd_num++;
//     }
    
//     // Clean up
//     free_cmd(cmd);
    
//     // Free the pipeline
//     temp = cmd1;
//     while(temp) {
//         t_cmd *next = temp->next;
//         free_cmd(temp);
//         temp = next;
//     }
    
//     return 0;
// }
