/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_id_login.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 11:10:16 by user42            #+#    #+#             */
/*   Updated: 2021/08/21 17:47:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

int	get_id_login(char *user)
{
	CURL *curl = curl_easy_init();
	struct curl_slist *list = NULL;
	char	buffer[1024];

	if(curl)
	{
		CURLcode res;
		sprintf(buffer, "https://api.intra.42.fr/v2/users/%s", user);
		curl_easy_setopt(curl, CURLOPT_URL, buffer);
		list = curl_slist_append(list, mytoken);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_id_login_callback);
		res = curl_easy_perform(curl);
		curl_slist_free_all(list);
		curl_easy_cleanup(curl);
		return (0);
	}
	return (1);
}
