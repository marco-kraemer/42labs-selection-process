/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 18:25:41 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 15:23:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/api.h"

size_t	save_token(char *contents, size_t size, size_t nmemb, void *userp)
{
	struct json_datas	json;

	size_t realsize = size * nmemb;
	json.parsed_json = json_tokener_parse(contents);
	json_object_object_get_ex(json.parsed_json, "access_token", &json.token);
	mytoken = ft_strjoin("Authorization: Bearer ", json_object_get_string(json.token));
	(void) userp;
	return realsize;
}

int	get_token(void)
{
	CURL *curl = curl_easy_init();

	if(curl)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.intra.42.fr/oauth/token");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ACCESS_API_INFO);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_token);
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
