/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/19 20:45:01 by maraurel         ###   ########.fr       */
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

void	save_info_db(char *content)
{
	char	buffer[1048];

	json.parsed_jsons = json_tokener_parse(content);
	json_object_object_get_ex(json.parsed_jsons, "correction_point", &json.correction_points); // GET CORRECTION POINTS
	json_object_object_get_ex(json.parsed_jsons, "wallet", &json.wallet); // GET COINS IN WALLET
	json_object_object_get_ex(json.parsed_jsons, "created_at", &json.start_date); // GET DATE USER HAS BEEN CREATED
	
	// GET NUMBER OF PROJECTS DONE (INCLUDING POOL)
	json_object_object_get_ex(json.parsed_jsons, "projects_users", &json.projects_users);
	int	n_projects = json_object_array_length(json.projects_users);
	
	// GET AVERAGE, LOWEST AND HIGHEST GRADE
	int	av_grade = 0, high_grade = 0, old_high_grade = 0, low_grade = 120, old_low_grade = 120;
	for (int i = 0; i < n_projects; i++)
	{
		json.project = json_object_array_get_idx(json.projects_users, i);
		json_object_object_get_ex(json.project, "final_mark", &json.project_grade);
		av_grade += json_object_get_int(json.project_grade);
		if (av_grade == (int)NULL)
			n_projects--;
		
		high_grade = json_object_get_int(json.project_grade);
		if (high_grade > old_high_grade)
			old_high_grade = high_grade;

		low_grade = json_object_get_int(json.project_grade);
		if (low_grade < old_low_grade)
			old_low_grade = low_grade;
	}
	low_grade = old_low_grade;
	high_grade = old_high_grade;
	av_grade = av_grade / n_projects;

	// GET DAYS TO FINISH A PROJECT
	char	*start_date = (char *)json_object_get_string(json.start_date);
	start_date = ft_substr(start_date, 0, 10);

	sprintf(buffer, "UPDATE students SET correction_points=%i, wallet=%i, start_date=\'%s\', num_projects=%i, av_grade=%i, highest_grade=%i, lowest_grade=%i  WHERE id=%i",
	json_object_get_int(json.correction_points), json_object_get_int(json.wallet),
	start_date, n_projects, av_grade, high_grade, low_grade, (int)data->id);
	query_mysql(con, buffer);
}

int	get_all_info(char *user)
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
		save_info_db(s.ptr);
		free(s.ptr);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
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

static void server(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
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
			get_all_info(login);
			char *correction_point = get_from_db("correction_points"); // GET CORRECTION_POINTS
			char *wallet = get_from_db("wallet"); // GET WALLET
			char *num_projects = get_from_db("num_projects"); // GET NUMBER OF PROJECTS DONE
			char *av_grade = get_from_db("av_grade"); // GET AVERAGE GRADE
			char *high_grade = get_from_db("highest_grade"); // GET HIGHEST GRADE
			char *low_grade = get_from_db("lowest_grade"); // LOWEST GRADE
			mg_http_reply(c, 200,"Content-Type: application/json\r\n",
			"{\"login\": %s, \"correction_points\": %s, \"wallet\": %s, \"number_of_projects_done\": %s, \"highest_grade\": %s, \"lowest_grade\": %s, \"average_grade\": %s}"
			, login, correction_point, wallet, num_projects, high_grade, low_grade, av_grade);
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

int	main(void)
{


	create_db(); // Create database
	get_token(); // Get Token
	struct mg_mgr	mgr; // Event manager
	mg_mgr_init(&mgr); // Initialise event manager
	mg_http_listen(&mgr, s_http_addr, server, NULL); // Create HTTP listener
	for (;;)
		mg_mgr_poll(&mgr, 1000); // Infinite event loop
	free(mytoken);
	mg_mgr_free(&mgr);
	return (0);
}
