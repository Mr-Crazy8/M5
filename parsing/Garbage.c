// #include "parsing.h"

// typedef struct s_collet
// {
//    char *adders;
//    void *add;
//    size_t size;
//    int in_use;
//    struct s_collet *next;
// } t_collet;

// void ft_free(void *addr, t_collet **addres_list)
// {
//     char *string_addr = addres_to_string(addr);
//     remove_node(string_addr, addres_list);
//     free(string_addr);
// }

// void remove_node(void *addr_str, t_collet **head)
// {
//     t_collet *curr = *head;
//     t_collet *prev = NULL;
    
//     while (curr)
//     {
//         if (strcmp(addr_str, curr->adders) == 0)
//         {
//             if (prev)
//                 prev->next = curr->next;
//             else
//                 *head = curr->next;
                
//             free(curr->adders); // Free the address string
//             free(curr->add);
//             free(curr);        // Free the node itself
//             return;
//         }
//         prev = curr;
//         curr = curr->next;
//     }
// }

// void ft_free_unused(t_collet **addres_list)
// {
//     t_collet *tmp;
//     t_collet *curr;
//     int found_unused;
//     found_unused = 1;
//     while (found_unused && *addres_list != NULL)
//     {
//         found_unused = 0;
//         curr = *addres_list;
        
//         while (curr)
//         {
//             tmp = curr;
//             curr = curr->next;
            
//             if (tmp->in_use <= 0)
//             {
//                 remove_node(tmp->adders, addres_list);
//                 found_unused = 1;
//                 break; 
//             }
//         }
//     }
// }


// void ft_in_use(void *adders, t_collet *addres_list)
// {
//    t_collet *tmp;

//    tmp = addres_list;
//    char *string_addres = addres_to_string(adders);
//    while (tmp)
//    {
//       if (strcmp(string_addres, tmp->adders) == 0)
//       {
//          tmp->in_use++;
//          break;
//       }
//       tmp = tmp->next;
//    }
//    free(string_addres);
// }

// void ft_not_in_use(void *adders, t_collet *addres_list)
// {
//    t_collet *tmp;

//    tmp = addres_list;
//    char *string_addres = addres_to_string(adders);
//    while (tmp)
//    {
//       if (strcmp(string_addres, tmp->adders) == 0)
//       {
//          tmp->in_use--;
//          if (tmp->in_use <= 0)
//          {
//             free(string_addres);
//             remove_node(tmp->adders, addres_list);
//             return;
//          }
//          break;
//       }
//       tmp = tmp->next;
//    }
//    free(string_addres);
// }

// void	ft_lstadd_back_addres(t_collet **lst, t_collet *new)
// {
// 	t_collet	*temp;

// 	if (lst == NULL || new == NULL)
// 		return ;
// 	if (*lst == NULL)
// 	{
// 		*lst = new;
// 		return ;
// 	}
// 	else
// 		temp = *lst;
// 	while (temp->next != NULL)
// 	{
// 		temp = temp->next;
// 	}
// 	temp->next = new;
// }


// t_collet	*addres_node_maker(void *addres, size_t size)
// {
//     t_collet	*addres_node;

//     addres_node = malloc(sizeof(t_collet));
//     if (!addres_node)
//         return NULL;
        
//    addres_node->adders = addres_to_string(addres);
//    addres_node->add = addres;
//    addres_node->size = size;
//    addres_node->in_use = 1;
        
//    addres_node->next = NULL;
//    return addres_node;

// }

// t_collet *collet(t_collet *addres_list, void *adders, size_t size)
// {
//    t_collet *new_node;
//    new_node = addres_node_maker(adders, size);

//    ft_lstadd_back_addres(&addres_list, new_node);

//    return addres_list;
// }


// void	*ft_malloc(t_collet **addres_list,size_t size)
// {
//    void *addres;

//    addres = malloc(size);
//    if (addres)
//       *addres_list = collet(*addres_list, addres, size);
//    return(addres);
// }




// static char	*addres_to_string_helper(unsigned long nbr, char *base, char *buffer, int *pos)
// {

// 	if (nbr >= 16)
// 		   addres_to_string_helper(nbr / 16, base, buffer, pos);
//    buffer[(*pos)] = base[nbr % 16];
//    (*pos)++;

// 	return (buffer);
// }

// char	*addres_to_string(void *ptr)
// {
// 	int				pos;
// 	char			*base;
// 	unsigned long	address;
//    char *buffer;
// 	pos = 0;

//    buffer = malloc(sizeof(char) *  19);
//    if (!buffer)
//       return (NULL);
// 	address = (unsigned long)ptr;
// 	base = "0123456789abcdef";

//    buffer[0] = '0';
//    buffer[1] = 'x';
//    pos = 2;
//    addres_to_string_helper(address, base, buffer, &pos);
//    buffer[pos] = '\0';
// 	return (buffer);
// }