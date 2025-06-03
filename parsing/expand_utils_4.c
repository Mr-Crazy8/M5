/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:46:00 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/01 11:47:46 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			total_size;
	void			*ptr;
	unsigned char	*b_ptr;
	size_t			i;

	i = 0;
	total_size = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(1));
	if (nmemb && size
		&& total_size / nmemb != size)
		return (NULL);
	ptr = malloc(total_size);
	if (ptr == NULL)
		return (NULL);
	b_ptr = (unsigned char *)ptr;
	while (i < total_size)
	{
		b_ptr[i] = 0;
		i++;
	}
	return (ptr);
}


static int	is_in_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i2;
	size_t	i1;
	size_t	r;
	size_t	len;
	char	*strtrim;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	i1 = 0;
	i2 = ft_strlen(s1);
	r = 0;
	while (s1[i1] != '\0' && is_in_set(s1[i1], set))
		i1++;
	while (i2 > i1 && is_in_set(s1[i2 - 1], set))
		i2--;
	len = i2 - i1;
	strtrim = (char *)malloc((len + 1) * sizeof(char));
	if (strtrim == NULL)
		return (NULL);
	while (i1 < i2)
		strtrim[r++] = s1[i1++];
	strtrim[r] = '\0';
	return (strtrim);
}


int	ft_isdigiti(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	else
		return (0);
}


char *chenger(char *str)
{
    int i = 0;

    if (!str)
        return NULL;
    while (str[i])
    {
        if (str[i] == '\'')
            str[i] = 10;
        else if (str[i] == '\"')
            str[i] = 11;
        i++;
    }
    return (str);

}