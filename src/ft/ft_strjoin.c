/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 09:02:37 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 15:23:21 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/api.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	int		i;
	int		len;

	len = strlen(s1) + strlen(s2);
	if (!s1 || !s2)
		return (NULL);
	if (!(p = malloc(len + 1)))
		return (NULL);
	i = 0;
	while (s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		p[strlen(s1) + i] = s2[i];
		i++;
	}
	p[strlen(s1) + i] = '\0';
	return (p);
}
