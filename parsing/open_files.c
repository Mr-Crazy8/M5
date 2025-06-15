/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:08:46 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 17:02:19 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	*open_file(t_cmd *cmd, int type, char *file, int Ambiguous)
{
	int	*fd;
	int	*fd_heredoc;

	fd = NULL;
	fd_heredoc = NULL;
	if (type == 3)
	{
		fd_heredoc = heredoc_opener();
		return (fd_heredoc);
	}
	fd = malloc(2 * sizeof(int));
	if (!fd)
		return (NULL);
	if (type == 0 && Ambiguous != 1)
	{
		fd[0] = open(file, O_RDONLY);
		fd[1] = -1;
		if (access(file, R_OK) == -1)
		{
			print_file_error(file, 3);
			cmd->data.exit_status = get_or_set(SET, 1);
		}
		else if (fd[0] == -1)
			print_file_error(file, 0);
	}
	if (type == 1 && Ambiguous != 1)
	{
		fd[0] = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		fd[1] = -1;
		if (access(file, W_OK) == -1)
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
		if (access(file, W_OK) == -1)
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
	return (fd);
}

void	print_file_error(char *file, int i)
{
	write(2, "minishell : ", 13);
	write(2, file, ft_strlen(file));
	if (i == 0)
		write(2, " : No such file or directory\n", 29);
	else if (i == 1)
		write(2, ": Cannot create or write to file\n", 33);
	else if (i == 2)
		write(2, ": Cannot append to file\n", 24);
	else if (i == 3)
	{
		write(2, ": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, "\n", 1);
	}
}

void	file_opener(t_cmd *cmd, t_env *env)
{
	t_cmd	*tmp;
	t_redir	*tp;
	int		*fd;
	int		failed;

	tmp = cmd;
	failed = 0;
	tp = NULL;
	while (tmp)
	{
		tp = tmp->redirs;
		failed = 0;
		while (tp)
		{
			if (failed == 0)
			{
				fd = open_file(cmd, tp->type, tp->file, tp->ambiguous);
				if (!fd)
				{
					fd = malloc(2 * sizeof(int));
					if (fd)
					{
						fd[0] = -1;
						fd[1] = -1;
					}
					tp->fd = fd;
					failed = 1;
				}
				else if (fd[0] == -1)
				{
					tp->fd = fd;
					failed = 1;
				}
				else
					tp->fd = fd;
			}
			else
			{
				fd = malloc(2 * sizeof(int));
				if (fd)
				{
					fd[0] = -1;
					fd[1] = -1;
					tp->fd = fd;
				}
				else
					tp->fd = NULL;
			}
			tp = tp->next;
		}
		tmp = tmp->next;
	}
}
