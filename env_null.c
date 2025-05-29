
#include "minishell.h"
t_env *creat_new_env(char *key, char *value, int for_path)
{
    t_env *new_node;

    new_node = malloc(sizeof(t_env));
    if(!new_node)
        return (NULL);
    new_node->key = key;
    new_node->value = value;
	new_node->for_path = for_path;
    new_node->next = NULL;
    return (new_node);
}
void env_null(t_env **list)
{
	t_env *tmp;

	// tmp = *list;
	tmp = creat_new_env("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.", 1);
	tmp->for_path = 1;
	ft_lstadd_back(list, tmp);
	ft_lstadd_back(list, creat_new_env("OLDPWD", NULL, 0));
    ft_lstadd_back(list, creat_new_env("PWD", getcwd(NULL, 0), 0));
    ft_lstadd_back(list, creat_new_env("SHELVL", "1", 0));
}
// void value_empty(t_env **env_list, char *key, char *value)
// {
// 	// t_env *tmp;
// 	t_env *new_node;

// 	// tmp = *env_list;
// 	// while(tmp)
// 	// {
// 	// 	if (strcmp(key, tmp->key) == 0)
// 	// 	{
// 	// 		free(tmp->value);
// 	// 		tmp->value = strdup(value);
// 	// 		tmp->is_not_active = 1;
// 	// 		return;
// 	// 	}
// 	// 	tmp = tmp->next;
// 	// }
// 	new_node = malloc(sizeof(t_env));
// 	if(!new_node)
// 		return;
// 	new_node->key = strdup(key);
// 	new_node->value = strdup(value);
// 	new_node->is_not_active = 1;
// 	new_node->next = NULL;
// 	ft_lstadd_back(env_list, new_node);
// }
void value_empty(t_env **env_list, char *key)
{
	// t_env *tmp;
	t_env *new_node;

	// tmp = *env_list;
	// while(tmp)
	// {
	// 	if (strcmp(key, tmp->key) == 0)
	// 	{
	// 		free(tmp->value);
	// 		tmp->value = strdup(value);
	// 		tmp->is_not_active = 1;
	// 		return;
	// 	}
	// 	tmp = tmp->next;
	// }
	new_node = malloc(sizeof(t_env));
	if(!new_node)
		return;
	new_node->key = strdup(key);
	new_node->value = NULL;
	new_node->is_not_active = 1;
	new_node->next = NULL;
	ft_lstadd_back(env_list, new_node);
}