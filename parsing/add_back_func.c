#include "parsing.h"

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	else
		temp = *lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = new;
}

void	ft_lstadd_back_token(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}


void	add_redir_back(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	add_cmd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

int get_or_set(int type, int status)
{
	static int exit_status = 0;
	if (type == SET)
		exit_status = status;
	return (exit_status);
}


int	is_valid_key(char *key)
{
	int i;

	if (!key || (!((key[0] >= 'a' && key[0] <= 'z') || 
	               (key[0] >= 'A' && key[0] <= 'Z') || 
	               key[0] == '_')))
		return (1);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!((key[i] >= 'a' && key[i] <= 'z') ||
		      (key[i] >= 'A' && key[i] <= 'Z') ||
		      (key[i] >= '0' && key[i] <= '9') ||
		      key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}


char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;
	size_t	l;

	i = 0;
	if (s == NULL)
		return (NULL);
	l = strlen(s);
	if (start >= l)
		return (strdup(""));
	if (len > l - start)
		len = l - start;
	ptr = (char *)malloc(sizeof(char) * len + 1);
	if (ptr == NULL)
		return (NULL);
	while (i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[len] = '\0';
	return (ptr);
}

