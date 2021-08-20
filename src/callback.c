/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 18:14:12 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/20 11:49:07 by maraurel         ###   ########.fr       */
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
	size_t realsize = size * nmemb;
	data = (struct data *) userp;
	
	data->content = contents;
	return realsize;
}

size_t	get_id_login_callback(char *content, size_t size, size_t nmemb, void *userp)
{
	char	buf[1024];

	data->id = -1;
	mjson_get_number(content, strlen(content), "$.id", &data->id);
	mjson_get_string(content, strlen(content), "$.login", buf, sizeof(buf));
	data->login = strdup(buf);
	if (data->id != -1)
	{
		sprintf(buf, "INSERT INTO students(id, login) VALUES(%i, \'%s\')", (int)data->id, data->login);
		query_mysql(con, buf);
	}
	free(data->login);
	(void) userp;
	(void) size;
	(void) nmemb;
	return (0);
}
