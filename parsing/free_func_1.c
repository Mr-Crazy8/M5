#include "parsing.h"

void free_redirs(t_redir *redir_list)
{
    t_redir *current;
    t_redir *next;

    current = redir_list;

    while(current)
    {
        next = current->next;
        if (current->file)
            free(current->file);
        if (current->orig_token)
            free(current->orig_token);
        free(current);
        current = next;
    }
}
void free_split_str(char **split_str)
{
    int j;

    j = 0;
    if (!split_str)  // Add this NULL check
        return;
    while (split_str[j])
        free(split_str[j++]);
    free(split_str);
}

void free_cmd_list_hp(char **str, char **str_befor)
{
    if (str)
    {
        free_split_str(str);
        str = NULL;
    }
    if (str_befor)
    {
        free_split_str(str_befor);
        str_befor = NULL;
    }

}
void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current;
    t_cmd *next;

    current = cmd_list;
    while (current)
    {
        next = current->next;
        if (current->cmd)
        {
            free(current->cmd);
            current->cmd = NULL;
        }
        free_cmd_list_hp(current->args, current->args_befor_quotes_remover);
        if (current->redirs)
        {
            free_redirs(current->redirs);
            current->redirs = NULL;
        }
        free(current);
        current = next;
    }
}
