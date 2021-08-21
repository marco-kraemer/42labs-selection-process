/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infos.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 11:08:51 by user42            #+#    #+#             */
/*   Updated: 2021/08/21 11:10:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

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
		if (av_grade == 0)
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