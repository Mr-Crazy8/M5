
// #include "minishell.h"

// char *check_path(t_env **list)
// {
//     t_env *tmp;

//     if (!list || !*list)
//         return (NULL);
        
//     tmp = *list;
//     while (tmp)
//     {
//         if (!ft_strcmp(tmp->key, "PATH"))
//             return (tmp->value);
//         tmp = tmp->next;
//     }
//     return (NULL);
// }

// void    ft_free_split(char **str)
// {
//     int i = 0;

//     while(str[i])
//     {
//         free(str[i]);
//         i++;
//     }
//     free(str);
// }

// void ft_excute_commands(t_cmd *cmd, t_env **list)
// {
//     char **help;
//     pid_t child_pid;
//     char **split_path;
//     char *helper;
//     char *path;
//     int i = 0;
//     int status;
//     help = convert_env_list(list);

//     if (!cmd->args || !cmd->args[0])
//         return;
//     if (ft_strchr(cmd->args[0], '/'))
//     {
//         child_pid = fork();
//         if (child_pid == 0)
//         {
//             if(!(access(cmd->args[0], X_OK) == -1))
//             {
//                 execve(cmd->args[0], cmd->args, help);
//                 cmd->data.exit_status = 0;
//             }
//             write(2, "minishell: ", 11);
//             write(2, cmd->args[0], ft_strlen(cmd->args[0]));
//             write(2, ": No such file or directory\n", 28);
//             cmd->data.exit_status = 127;
//         }
//         else if (child_pid > 0)
//             waitpid(child_pid, &status, 0);
//             cmd->data.exit_status = WEXITSTATUS(status);
//         return ;
//     }
//     path = check_path(list);
//     if (!path)
//     {
//         write(2, "PATH not found\n", 15);
//         return;
//     }
    
//     split_path = ft_split(path, ':');
//     if (!split_path)
//         return;
        
//     child_pid = fork();
//     if (child_pid == 0)
//     {
//         while (split_path[i])
//         {
//             char *tmp = ft_strjoin(split_path[i], "/");
//             helper = ft_strjoin(tmp, cmd->args[0]);
//             free(tmp);
//             if (!helper)
//             {
//                 i++;
//                 continue;
//             }
//             if(!(access(helper, X_OK) == -1))
//             {
//                 execve(helper, cmd->args, help);
//             }
//             free(helper);
//             i++;
//         }
//         i = 0;
//         write(2, "minishell: ", 11);
//         write(2, cmd->args[0], ft_strlen(cmd->args[0]));
//         write(2, ": command not found\n", 20);
//         ft_free_split(split_path);
//         exit(127);
//     }
//     else if (child_pid > 0)
//     {
//         waitpid(child_pid, &status, 0);
//         cmd->data.exit_status = WEXITSTATUS(status);
//         printf("%d\n", cmd->data.exit_status);
//         ft_free_split(split_path);
//     }
//     else
//     {
//         perror("fork failed");
//         ft_free_split(split_path);
//     }
// }