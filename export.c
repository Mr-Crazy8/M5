#include "minishell.h"

// int	is_valid_key(char *key)
// {
// 	int i;

// 	if (!key || (!((key[0] >= 'a' && key[0] <= 'z') || 
// 	               (key[0] >= 'A' && key[0] <= 'Z') || 
// 	               key[0] == '_')))
// 		return (1);

// 	i = 1;
// 	while (key[i] && key[i] != '=')
// 	{
// 		if (!((key[i] >= 'a' && key[i] <= 'z') ||
// 		      (key[i] >= 'A' && key[i] <= 'Z') ||
// 		      (key[i] >= '0' && key[i] <= '9') ||
// 		      key[i] == '_'))
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

t_env *sort_env(t_env **head)
{
    t_env *helper = NULL;
    char *tmp_key;
    char *tmp_value;
    helper = *head;
    if(!*head || !head)
        return (NULL);
    while(*head && (*head)->next != NULL)
    {
        if(ft_strcmp((*head)->key, (*head)->next->key) > 0)
        {
            tmp_key = (*head)->key;
            tmp_value = (*head)->value;
            (*head)->key = (*head)->next->key;
            (*head)->value = (*head)->next->value;
            (*head)->next->key = tmp_key;
            (*head)->next->value = tmp_value;
            *head = helper;
        }
        else
            *head = (*head)->next;
    }
    return (helper);
}

char *get_value(char *str)
{
    int i = 0;
    int tmp = 0;
    unsigned int len = 0;
    char *helper = NULL;
    while(str[i] && str[i] != '=' && str[i] != '+')
    i++;
    if(str[i] == '+')
        i++;
    if(str[i] == '=')
        i++;
    tmp = i;
    while(str[i])
    {
        len++;
        i++;
    }
    helper = ft_substr(str, tmp, len);
    if (!helper)
        return (NULL);
    return (helper);
}
char    *get_key(char *str)
{
    unsigned int i = 0;
    while(str[i])
    {
        if(str[i] == '+' && str[i + 1] == '=')
        break ;
        if (str[i] == '=')
        break ;
        i++;
    }
    char *helper = ft_substr(str, 0, i);
    if(!helper)
        return (NULL);
    return (helper);
}

int check_append(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '+' && str[i + 1] == '=')
        {
            return ('+');
        }
        else if (str[i] == '=')
        {
            return ('=');
        }
        i++;
    }
    return (0);
}

void    set_env(t_env **env, char *key, char *value)
{
    t_env *tmp;
    t_env *new_node = NULL;

	tmp = *env;
	while(tmp)
	{
        if(ft_strcmp(tmp->key, key) == 0)
		{
            if (tmp->value)
                tmp->value = ft_strjoin(tmp->value, value);
            tmp->is_not_active = 0;
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
	ft_lstadd_back(env, new_node);
}

int ft_check(t_env *env_list, char *key)
{
    t_env *tmp;
    
    tmp = env_list;
    
    while(tmp)
    {
        if(ft_strcmp(key, tmp->key) == 0)
        {
            return (1);
        }
        tmp = tmp->next;
    }
    return (0);
}
int ft_handel_export(char **str, t_env **lst)
{
    int i = 0;
    char *key = NULL;
    char *value = NULL;
    int status = 0;
    while(str[i])
    {
        key = get_key(str[i]);
        if(is_valid_key(key) == 1)
        {
            printf("minishell: export: %s : not valid identifier\n", str[i]);
            status = 1;
            free(key);
            i++;
            continue;
        }
        value = get_value(str[i]);
        if((ft_strchr(str[i], '=') == NULL))
        {
           if (ft_check(*lst, key) == 1)
           {
                printf("%s\n", str[i]);
                i++;
                continue;
           }
        }
        if(check_append(str[i]) == '+')
        {
            puts("jjjj");
            set_env(lst, key, value);
        }
        if (check_append(str[i]) == '=')
        {
            updat_env(lst, key, value);
        }
        if(check_append(str[i]) == 0)
        {
            value_empty(lst, key);
        }
        free(key);
        free(value);
        i++;
    } 
    return (status);
}
void print_export(t_env *env_list)
{
    t_env *tmp = env_list;
    
    while(tmp)
    {
        if(tmp->value)
        {
            printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
        }
        else
            printf("declare -x %s\n", tmp->key);
        tmp = tmp->next;
    }  
}
int ft_export(char **str, t_env **env_list)
{
    int status;
    
    status  = 0;
    *env_list = sort_env(env_list);
    if(!str[1])
    {
        print_export(*env_list);
        return (0);
    }
    else
    {
        status = ft_handel_export(str + 1, env_list);
        return (status);     
    }
}