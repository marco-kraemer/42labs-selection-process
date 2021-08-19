/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 23:42:26 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/18 23:45:01 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	i;

	if (!s)
		return (NULL);
	if (!(p = malloc(len + 1)))
		return (NULL);
	i = 0;
	if (start < strlen(s))
	{
		while (i < len && s[start])
		{
			p[i] = s[start];
			i++;
			start++;
		}
	}
	p[i] = '\0';
	return (p);
}
