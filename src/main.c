/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/18 23:23:58 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

static const char *s_http_addr = "http://0.0.0.0:3000";
static const char *s_root_dir = ".";

struct json_datas	json;
struct data 		*data;
MYSQL 			*con;
int			ret_value;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		if (mg_http_match_uri(hm, "/maraurel"))
		{
			query_mysql(con, "SELECT login FROM students");
			MYSQL_RES *result = mysql_store_result(con);
			MYSQL_ROW row = mysql_fetch_row(result);
			mg_http_reply(c, 200,"Content-Type: application/json\r\n", "{\"result\": %s}", row[0]);
		}
		else
		{
			mg_http_reply(c, 404, "", "Student not found\n", (int) hm->uri.len, hm->uri.ptr);
		}
		
	}
	(void) fn_data;
	(void) s_root_dir;
}

static size_t	get_token_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	data = (struct data *) userp;

	data->content = contents;
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
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_token_callback);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform failed\n");
			return (1);
		}
		curl_easy_cleanup(curl);
		return (0);
	}
	return (1);
}

static size_t	get_id_callback(char *content, size_t size, size_t nmemb, void *userp)
{
	char	buf[1024];

	mjson_get_number(content, strlen(content), "$.id", &data->id);
	mjson_get_string(content, strlen(content), "$.login", buf, sizeof(buf));
	data->login = strdup(buf);
	sprintf(buf, "INSERT INTO students VALUES(%i, \'%s\', %s)", (int)data->id, data->login, "NULL");
	query_mysql(con, buf);
	free(data->login);
	(void)size;
	(void)nmemb;
	(void)userp;
	return 0;
}

int	get_information(char *mytoken)
{
	CURL *curl = curl_easy_init();
	struct curl_slist *list = NULL;

	if(curl)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.intra.42.fr/v2/users/maraurel");
		list = curl_slist_append(list, mytoken);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_id_callback);
		res = curl_easy_perform(curl);
		curl_slist_free_all(list);
		curl_easy_cleanup(curl);
		return (0);
	}
	return (1);
}

void	query_mysql(MYSQL *con, const char *s)
{
	if (mysql_query(con, s))
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
}

int main(void)
{

	char	*mytoken;

	con = mysql_init(NULL);
	// Create database
	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	if (mysql_real_connect(con, "localhost", "root", "Password1$", NULL, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	query_mysql(con, "CREATE DATABASE IF NOT EXISTS api");
	query_mysql(con, "USE api");
	query_mysql(con, "DROP TABLE IF EXISTS students");
	query_mysql(con, "CREATE TABLE students(id INT, login VARCHAR(30), projects_done TINYINT);");

	// Get Token
	get_token();
	json.parsed_json = json_tokener_parse(data->content);
	json_object_object_get_ex(json.parsed_json, "access_token", &json.token);
	mytoken = ft_strjoin("Authorization: Bearer ", json_object_get_string(json.token));
	
	// Get all intra information
	get_information(mytoken);
	
	// Parser information using json
	// Todo
	
	// Send informations to a database
	// TODO


	// Event manager
	struct mg_mgr	mgr;
	// Initialise event manager
	mg_mgr_init(&mgr);
	// Create HTTP listener
	mg_http_listen(&mgr, s_http_addr, fn, NULL);
	// Infinite event loop
	for (;;)
		mg_mgr_poll(&mgr, 1000);
	// Free allocated memory
	free(mytoken);
	mg_mgr_free(&mgr);
	return 0;
}
