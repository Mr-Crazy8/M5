/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:15 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/26 18:01:45 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	copy_strings(char *ptr, const char *s1, const char *s2)
// {
// 	unsigned int	i;
// 	unsigned int	j;

// 	i = 0;
// 	while (s1[i] != '\0')
// 	{
// 		ptr[i] = s1[i];
// 		i++;
// 	}
// 	j = 0;
// 	while (s2[j] != '\0')
// 	{
// 		ptr[i + j] = s2[j];
// 		j++;
// 	}
// 	ptr[i + j] = '\0';
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char			*ptr;
// 	size_t			total_len;

// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (!s1)
// 		return (strdup(s2));
// 	if (!s2)
// 		return (strdup(s1));
// 	total_len = strlen(s1) + strlen(s2);
// 	ptr = (char *)malloc (sizeof(char) * (total_len + 1));
// 	if (ptr == NULL)
// 		return (NULL);
// 	copy_strings(ptr, s1, s2);
// 	return (ptr);
// }

char *get_value_env(char *key, t_env **list)
{
	t_env   *tmp;

	tmp = *list;
	while(tmp)
	{
		if(ft_strcmp(tmp->key, key) == 0)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
void    set_value_env(t_env **env, char *key, char *value)
{
	t_env *tmp;

	tmp = *env;
	while(tmp)
	{
		if(ft_strcmp(tmp->key, key) == 0)
		{
			if (tmp->value)
			{
				free(tmp->value);
				tmp->value = NULL;
			}
			tmp->value = ft_strjoin(tmp->value, value);
			return ;
		}
		tmp = tmp->next;
	}
}

void updat_env(t_env **env_list, char *key, char *value)
{
	t_env *tmp;
	t_env *new_node;

	tmp = *env_list;
	while(tmp)
	{
		if (strcmp(key, tmp->key) == 0)
		{
			free(tmp->value);
			tmp->value = strdup(value);
			tmp->is_not_active = 0;
			return;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if(!new_node)
		return;
	new_node->key = strdup(key);
	new_node->value = strdup(value);
	new_node->is_not_active = 0;
	new_node->next = NULL;
	ft_lstadd_back(env_list, new_node);
}

char *get_path(char *str, t_env **list)
{
	char *path;

	path = NULL;
	if(str == NULL || str[0] == '~')
	{
		path = get_value_env(strdup("HOME"), list);
		if(!path)
			return(NULL);
	}
	else if(!path)
	{
		path = str;
	}
	return (path);
}
char *check_pwd(t_env **list, char *str, char *old_pwd)
{
	static char *new_pwd;
	char *tmp;

    write(2, "minishell: cd: error retrieving current directory\n", 50);
	if (old_pwd[ft_strlen(old_pwd) - 1] != '/')
	{
		tmp = ft_strjoin(old_pwd, "/");
		printf("%s\n", tmp);
		if(!tmp)
			return (NULL);
		new_pwd = ft_strjoin(tmp, str);
		free(tmp);
	}
	else
	{
		new_pwd = ft_strjoin(old_pwd, str);
		if(!new_pwd)
			return (NULL);
	}
	updat_env(list, "PWD", new_pwd);
	return (new_pwd);
}
int ft_cd(char **str, t_env **list, t_data data)
{
    char *cwd;
    char *new_cwd;
    char *value;
	char *new_pwd;

    new_cwd = get_path(str[1], list);
    value = get_value_env("PWD", list);
    if (!new_cwd || chdir(new_cwd) == -1)
    {
		// ft_putstr_fd("minishell: ", 2);
		strerror(errno);
		return(1);
    }
    if (value)
        updat_env(list, "OLDPWD", value);

    cwd = getcwd(NULL, 0);
    if (!cwd)
	{
		new_pwd = check_pwd(list, str[1], value);
		data.new_pwd = new_pwd;
	}
    else
    {
        set_value_env(list, "PWD", cwd);
        free(cwd);
    }
    return (0);
}

// int	ft_cd(char **str, t_env **list)
// {
// 	char	*cwd;
// 	char	*new_cwd;
// 	char	*value;
// 	static new_pwd;

// 	new_cwd = get_path(str[1], list);
// 	value = get_value_env("PWD", list); // detsktop//cccc/0/1/2;
// 	if(!new_cwd || chdir(new_cwd) == -1)
// 	{
// 		write(2, "minishell: cd: HOME not set\n", 28);
// 		return (1);
// 	}
// 	if(value)
// 		updat_env(list, "OLDPWD", value);
// 	// set_value_env(list, "OLDPWD", value);
// 	cwd = getcwd(NULL, 0);
// 	if(!cwd)
// 	{
// 		check_pwd(new_pwd, cwd, value);
// 	}
// 	set_value_env(list, "PWD", cwd);
// 	free(cwd);
// 	return (0);
// }