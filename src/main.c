/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/20 12:03:50 by maraurel         ###   ########.fr       */
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

	json.parsed_jsons = json_tokener_parse(content);
	json_object_object_get_ex(json.parsed_jsons, "correction_point", &json.correction_points); // GET CORRECTION POINTS
	json_object_object_get_ex(json.parsed_jsons, "wallet", &json.wallet); // GET COINS IN WALLET
	json_object_object_get_ex(json.parsed_jsons, "created_at", &json.start_date); // GET DATE USER HAS BEEN CREATED
	json_object_object_get_ex(json.parsed_jsons, "updated_at", &json.updated_date);

	// GET NUMBER OF PROJECTS DONE (INCLUDING POOL)
	json_object_object_get_ex(json.parsed_jsons, "projects_users", &json.projects_users);
	int	n_projects = json_object_array_length(json.projects_users);
	
	// GET AVERAGE, LOWEST AND HIGHEST GRADE
	int	av_grade = 0, high_grade = 0, old_high_grade = 0, low_grade = 120, old_low_grade = 120, n_occurrence = 0;
	for (int i = 0; i < n_projects; i++)
	{
		json.project = json_object_array_get_idx(json.projects_users, i);
		json_object_object_get_ex(json.project, "final_mark", &json.project_grade);
		json_object_object_get_ex(json.project, "occurrence", &json.occurrence);
		n_occurrence += json_object_get_int(json.occurrence) + 1;
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


	// GET LEVEL
	json_object_object_get_ex(json.parsed_jsons, "cursus_users", &json.cursus_users);
	json.cursus_users = json_object_array_get_idx(json.cursus_users, 0);
	json_object_object_get_ex(json.cursus_users, "level", &json.level);

	// GET NUM OF ACHIEVEMENTS
	json_object_object_get_ex(json.parsed_jsons, "achievements", &json.achievements);
	int n_achievements = json_object_array_length(json.achievements);


	// DATES
	struct date	start;
	char	*start_date = (char *)json_object_get_string(json.start_date);
	start.year = atoi(ft_substr(start_date, 0, 4));
	start.month = atoi(ft_substr(start_date, 5, 7));
	start.day = atoi(ft_substr(start_date, 8, 10));

	struct date	now;
	char	*today_date = (char *)json_object_get_string(json.updated_date);
	now.year = atoi(ft_substr(today_date, 0, 4));
	now.month = atoi(ft_substr(today_date, 5, 7));
	now.day = atoi(ft_substr(today_date, 8, 10));

	struct Date2 dt1 = {start.day, start.month, start.year};
	struct Date2 dt2 = {now.day, now.month, now.year};
	int	days_diff = getDifferenceDates(dt1, dt2);

	// PUT ALL INFORMATION IN DATABASE
	char	buffer[2096];
	sprintf(buffer, "UPDATE students SET correction_points=%i, wallet=%i, num_projects=%i, total_attempts=%i, av_grade=%i, highest_grade=%i, lowest_grade=%i, days_in_42=%i, level=%f, num_achievements=%i WHERE id=%i",
	json_object_get_int(json.correction_points), json_object_get_int(json.wallet),
	n_projects, n_occurrence, av_grade, high_grade, low_grade, days_diff,
	json_object_get_double(json.level), n_achievements, (int)data->id);
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
	//	query_mysql(con, "DELETE FROM students");
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
	char buffer[1048];

	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		char	*login = ft_substr(hm->uri.ptr, 1, hm->uri.len - 1);
		get_id_login(login);
		if (data->id == -1) // IF LOGIN DOESN'T MATCH
			mg_http_reply(c, 404, "", "Student not found\n", (int) hm->uri.len, hm->uri.ptr);
		else
		{
			get_all_info(login);
			int correction_point = atoi(get_from_db("correction_points")); // GET CORRECTION_POINTS
			int wallet = atoi(get_from_db("wallet")); // GET WALLET
			int num_projects = atoi(get_from_db("num_projects")); // GET NUMBER OF PROJECTS DONE
			int total_attempts = atoi(get_from_db("total_attempts")); // TOTAL ATTEMPS
			int av_grade = atoi(get_from_db("av_grade")); // GET AVERAGE GRADE
			int high_grade = atoi(get_from_db("highest_grade")); // GET HIGHEST GRADE
			int low_grade = atoi(get_from_db("lowest_grade")); // LOWEST GRADE
			int days42 = atoi(get_from_db("days_in_42"));
			char *level = get_from_db("level"); // LEVEL
			int num_achievements = atoi(get_from_db("num_achievements")); // NUMBER OF ACHIEVEMENTS
			mg_http_reply(c, 200,"Content-Type: application/json\r\n",
			"{\"login\": %s, \"correction_points\": %i, \"wallet\": %i, \"average_wallet_increase_per_project\": %f, \"number_of_projects_done\": %i, \"number_of_attempts\":%i, \"highest_grade\": %i, \"lowest_grade\": %i, \"average_grade\": %i, \"days_in_42\":%i, \"days_finish_a_project\": %f, \"hours_finish_a_project\": %f, \"average_number_of_attempts_per_project\": %f, \"level\": %s, \"number_achievements\": %i}"
			, login, correction_point, wallet, ((float)wallet / (float)num_projects), num_projects, total_attempts, high_grade, low_grade, av_grade,
			days42, ((float)days42 / (float)num_projects), (((float)days42 / (float)num_projects) * (float)24),
			((float)total_attempts / (float) num_projects), level, num_achievements);
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
