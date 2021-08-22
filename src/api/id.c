/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   id.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 11:10:16 by user42            #+#    #+#             */
/*   Updated: 2021/08/22 15:23:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/api.h"

void	save_id(char *content)
{
	char	buf[1024];
	int	n_row;

	id = -1;
	mjson_get_number(content, strlen(content), "$.id", &id);
	sprintf(buf, "SELECT id FROM stats WHERE id=%i", (int)id);
	query_mysql(con, buf);
	MYSQL_RES *result = mysql_store_result(con);
	n_row = mysql_num_rows(result);
	if (id != -1 && n_row == 0)
	{
		sprintf(buf, "INSERT INTO stats(id) VALUES(%i)", (int)id);
		query_mysql(con, buf);
		sprintf(buf, "INSERT INTO user(id) VALUES(%i)", (int)id);
		query_mysql(con, buf);
	}
}

int	get_id_login(char *user)
{
	CURL *curl = curl_easy_init();
	struct curl_slist *list = NULL;
	char	buffer[1024];

	if(curl)
	{
		struct string s;
		init_string(&s);
		sprintf(buffer, "https://api.intra.42.fr/v2/users/%s", user);
		curl_easy_setopt(curl, CURLOPT_URL, buffer);
		list = curl_slist_append(list, mytoken);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_perform(curl);
		save_id(s.ptr);
		free(s.ptr);
		curl_easy_cleanup(curl);
		curl_slist_free_all(list);
		return (0);
	}
	return (1);
}
