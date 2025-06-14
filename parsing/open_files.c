#include "parsing.h"


 /* 0:<, 1:>, 2:>>, 3:<< */

/* O_RDONLY        open for reading only
   O_WRONLY        open for writing only
   O_RDWR          open for reading and writing
   O_NONBLOCK      do not block on open or for data to become available
   O_APPEND        append on each write
   O_CREAT         create file if it does not exist
   O_TRUNC         truncate size to 0
   O_EXCL          error if O_CREAT and the file exists
   O_SHLOCK        atomically obtain a shared lock
   O_EXLOCK        atomically obtain an exclusive lock
   O_NOFOLLOW      do not follow symlinks
   O_SYMLINK       allow open of symlinks
   O_EVTONLY       descriptor requested for event notifications only
   O_CLOEXEC       mark as close-on-exec */

int *open_file(t_cmd *cmd, int type, char *file, int Ambiguous)
{
    int *fd = NULL;
    int *fd_heredoc;
    fd = malloc(2 * sizeof(int));


    if (type == 3)
    {
       fd_heredoc = heredoc_opener();
       free(fd);
       return fd_heredoc;

    }
    if (type == 0 && Ambiguous != 1)
    {
        fd[0] = open(file, O_RDONLY);
        fd[1] = -1;
        if(access(file, R_OK) == -1)
        {
            print_file_error(file, 3);
            cmd->data.exit_status = get_or_set(SET, 1);
        }
        else if (fd[0] == -1)
        {
            print_file_error(file, 0);

        }
    }
    if (type == 1 && Ambiguous != 1)
    {
        fd[0] = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
        fd[1] = -1;
        if(access(file, W_OK) == -1)
        {
            print_file_error(file, 3);
            cmd->data.exit_status = get_or_set(SET, 1);
        }
        else if (fd[0] == -1)
            print_file_error(file, 1);
        
    }
    if (type == 2 && Ambiguous != 1)
    {
        fd[0] = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
        fd[1] = -1;
        if(access(file, W_OK) == -1)
        {
            print_file_error(file, 3);
            cmd->data.exit_status = get_or_set(SET, 1);
        }
        if (fd[0] == -1)
            print_file_error(file, 2);
    }
    if (type != 3 && Ambiguous == 1)
    {
        fd[0] = -1;
        fd[1] = -1;
    }
        
    return fd;
}

void print_file_error(char *file, int i)
{
    write(2, "minishell : ", 13);
    write(2, file, ft_strlen(file));
    if (i == 0)
        write(2, " : No such file or directory\n", 29);
    else if (i == 1)
        write(2, ": Cannot create or write to file\n", 33);
    else if (i == 2)
        write(2, ": Cannot append to file\n", 24);
    else if(i == 3)
    {
        write(2, ": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        write(2, "\n", 1);
    }
}



void file_opener(t_cmd *cmd, t_env *env)
{
    t_cmd *tmp;
    t_redir *tp = NULL;
    int *fd;
    int failed = 0;
    tmp = cmd;

    while (tmp)
    {
        tp = tmp->redirs;
        while (tp)
        {   
            
            if (failed == 0)
                fd = open_file(cmd, tp->type, tp->file, tp->Ambiguous);
            if (fd[0] == -1 && failed == 0)
            {
                tp->fd = fd;
                failed = 1;
            }
            else if (failed == 1)
            {
                    fd = malloc(2 * sizeof(int));
                    fd[0] = -1;
                    fd[1] = -1;
                    tp->fd = fd;

            }
            else
            {
                tp->fd = fd;
            }
            tp = tp->next;
        }
        tmp = tmp->next;
    }
}

