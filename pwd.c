/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:34:07 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/19 15:56:08 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pwd(t_data data)
{
	char	*pwd;
	int		status;

	status = 0;
	pwd = getcwd(NULL, 0);
	if(!pwd)
	{
		pwd = data.new_pwd;
		printf("%s\n", pwd);
		return(status);
	}
	else
		printf("%s\n", pwd);
	free(pwd);
	return (status);
}
