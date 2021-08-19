/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/19 18:11:01 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

static const char *s_http_addr = "http://0.0.0.0:3000";
static const char *s_root_dir = ".";
struct json_datas	json;
struct data 		*data;
MYSQL 			*con;
char			*mytoken;

void init_string(struct string *s)
{
	s->len = 0;
	s->ptr = malloc(s->len+1);
	if (s->ptr == NULL) {
	fprintf(stderr, "malloc() failed\n");
	exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

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

	return size*nmemb;
}

void	save_datas_db(char *content)
{
	char	buffer[1048];

	json.parsed_jsons = json_tokener_parse(content);

	// GET CORRECTION POINTS
	json_object_object_get_ex(json.parsed_jsons, "correction_point", &json.correction_points);
	// GET COINS IN WALLET
	json_object_object_get_ex(json.parsed_jsons, "wallet", &json.wallet);
	// GET DATA USER HAS BEEN CREATED
	json_object_object_get_ex(json.parsed_jsons, "created_at", &json.start_date);
	
	// GET NUMBER OF PROJECTS ENROLLED (INCLUDING POOL)
	json_object_object_get_ex(json.parsed_jsons, "projects_users", &json.projects_users);
	int	n_projects = json_object_array_length(json.projects_users);
	
	// GET AVERAGE GRADE
	int	av_grade = 0;
	for (int i = 0; i < n_projects; i++)
	{
		json.project = json_object_array_get_idx(json.projects_users, i);
		json_object_object_get_ex(json.project, "final_mark", &json.project_grade);
		av_grade += json_object_get_int(json.project_grade);
		if (av_grade == (int)NULL)
			n_projects--;
	}
	av_grade = av_grade / n_projects;

	sprintf(buffer, "UPDATE students SET correction_points=%i, wallet=%i, start_date=\'%s\', num_projects=%i, av_grade=%i WHERE id=%i",
	json_object_get_int(json.correction_points), json_object_get_int(json.wallet),
	json_object_get_string(json.start_date), n_projects, av_grade, (int)data->id);
	query_mysql(con, buffer);
}

int	get_datas(char *user)
{
	CURLcode res;
	struct curl_slist *list = NULL;
	char	buffer[1024];

	CURL *curl = curl_easy_init();
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
		res = curl_easy_perform(curl);
		save_datas_db(s.ptr);
		free(s.ptr);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
}

char	*get_from_db(char *column)
{
	char	buffer[1048];

	sprintf(buffer, "SELECT %s FROM students", column);
	query_mysql(con, buffer);
	MYSQL_RES *result = mysql_store_result(con);
	MYSQL_ROW row = mysql_fetch_row(result);
	return (row[0]);
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		char	*login = ft_substr(hm->uri.ptr, 1, hm->uri.len - 1);
		get_id_login(login);
		query_mysql(con, "SELECT login FROM students");
		MYSQL_RES *result = mysql_store_result(con);
		MYSQL_ROW row = mysql_fetch_row(result);
		if (strcmp(login, row[0]) == 0)
		{
			get_datas(login);
			char *correction_point = get_from_db("correction_points"); // GET CORRECTION_POINTS
			char *wallet = get_from_db("wallet"); // GET WALLET
			char *num_projects = get_from_db("num_projects");
			char *av_grade = get_from_db("av_grade");
			mg_http_reply(c, 200,"Content-Type: application/json\r\n",
			"{\"login\": %s, \"correction_points\": %s, \"wallet\": %s, \"number_of_projects_done\": %s, \"average_grade\": %s}"
			, login, correction_point, wallet, num_projects, av_grade);
		}
		else
		{
			mg_http_reply(c, 404, "", "Student not found\n", (int) hm->uri.len, hm->uri.ptr);
		}
		free(login);
		
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

static size_t	get_id_login_callback(char *content, size_t size, size_t nmemb, void *userp)
{
	char	buf[1024];

	mjson_get_number(content, strlen(content), "$.id", &data->id);
	mjson_get_string(content, strlen(content), "$.login", buf, sizeof(buf));
	data->login = strdup(buf);
	sprintf(buf, "INSERT INTO students(id, login) VALUES(%i, \'%s\')", (int)data->id, data->login);
	query_mysql(con, buf);
	free(data->login);
	(void) userp;
	(void) size;
	(void) nmemb;
	return (0);
}

int	get_id_login(char *user)
{
	CURL *curl = curl_easy_init();
	struct curl_slist *list = NULL;
	char	buffer[1024];

	if(curl)
	{
		CURLcode res;
		query_mysql(con, "DELETE FROM students");
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
	query_mysql(con, "CREATE TABLE students(id INT, login VARCHAR(30), correction_points SMALLINT, wallet INT, start_date VARCHAR(30), num_projects SMALLINT, av_grade TINYINT);");

	// Get Token
	get_token();
	json.parsed_json = json_tokener_parse(data->content);
	json_object_object_get_ex(json.parsed_json, "access_token", &json.token);
	mytoken = ft_strjoin("Authorization: Bearer ", json_object_get_string(json.token));
	
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
