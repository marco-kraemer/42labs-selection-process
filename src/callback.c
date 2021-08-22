/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 18:14:12 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 14:29:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
	fprintf(stderr, "realloc() failed\n");
	exit(EXIT_FAILURE);
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return (size * nmemb);
}

size_t	get_token_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
	struct json_datas	json;

	user = (struct user *) userp;	
	json.parsed_json = json_tokener_parse(contents);
	json_object_object_get_ex(json.parsed_json, "access_token", &json.token);
	mytoken = ft_strjoin("Authorization: Bearer ", json_object_get_string(json.token));
	return (size * nmemb);
}

size_t	get_id_login_callback(char *content, size_t size, size_t nmemb, void *userp)
{
	char	buf[1024];
	int	n_row;

	user->id = -1;
	mjson_get_number(content, strlen(content), "$.id", &user->id);
	sprintf(buf, "SELECT id FROM stats WHERE id=%i", (int)user->id);
	query_mysql(con, buf);
	MYSQL_RES *result = mysql_store_result(con);
	n_row = mysql_num_rows(result);
	if (user->id != -1 && n_row == 0)
	{
		sprintf(buf, "INSERT INTO stats(id) VALUES(%i)", (int)user->id);
		query_mysql(con, buf);
		sprintf(buf, "INSERT INTO user(id) VALUES(%i)", (int)user->id);
		query_mysql(con, buf);
	}
	(void) userp;
	(void) size;
	(void) nmemb;
	return (0);
}
