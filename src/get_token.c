/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 18:25:41 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 09:50:29 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

int	get_token(void)
{
	CURL *curl = curl_easy_init();

	if(curl)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.intra.42.fr/oauth/token");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ACCESS_API_INFO);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_token_callback);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform failed\n");
			return (1);
		}
		curl_easy_cleanup(curl);
	}
	return (0);
}
