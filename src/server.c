/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 11:04:27 by user42            #+#    #+#             */
/*   Updated: 2021/08/22 11:24:12 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

void	save_stats(struct Data *data)
{
	data->correction_points = atoi(get_from_db("stats", "correction_points"));
	data->wallet = atoi(get_from_db("stats", "wallet"));
	data->num_projects = atoi(get_from_db("stats", "num_projects"));
	data->av_wallet_increase_per_project = (float)data->wallet / (float)data->num_projects;
	data->total_attempts = atoi(get_from_db("stats", "total_attempts"));
	data->av_grade = atoi(get_from_db("stats", "av_grade"));
	data->high_grade = atoi(get_from_db("stats", "highest_grade"));
	data->low_grade = atoi(get_from_db("stats", "lowest_grade"));
	data->days42 = atoi(get_from_db("stats", "days_in_42"));
	data->days_finish_project = (float)data->days42 / (float)data->num_projects;
	data->hours_finish_project = (float)data->days_finish_project * (float)24;
	data->attemps_per_project = (float)data->total_attempts / (float) data->num_projects;
	data->level = get_from_db("stats", "level");
	data->num_achievements = atoi(get_from_db("stats", "num_achievements"));	
}

void	save_user(void)
{
	user->login = get_from_db("user", "login");
	user->full_name = get_from_db("user", "full_name");
	user->email = get_from_db("user", "email");
	user->campus_country = get_from_db("user", "campus_country");
	user->campus_city = get_from_db("user", "campus_city");
	return ;
}

void server(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	struct Data	data;

	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		char	*login = ft_substr(hm->uri.ptr, 1, hm->uri.len - 1);
		get_id_login(login);
		if (user->id == -1)
			mg_http_reply(c, 404,"Content-Type: application/json\r\n", "{\"error\": 404, \"message\": student_%s_not_found}", login);
		else
		{
			get_all_info(login);
			save_user();
			save_stats(&data);
			mg_http_reply(c, 200, "Content-Type: application/json\r\n",
			"{\"user\": [{\"login\": \"%s\", \"id\": %i, \"full_name\": \"%s\", \"email\": \'%s\', \"campus_city\": \"%s\", \"campus_country\": \"%s\"}],\"stats\":[{\"correction_points\": %i, \"wallet\": %i, \"average_wallet_increase_per_project\": %f, \"number_of_projects_done\": %i, \"number_of_attempts\":%i, \"highest_grade\": %i, \"lowest_grade\": %i, \"average_grade\": %i, \"days_in_42\":%i, \"days_finish_a_project\": %f, \"hours_finish_a_project\": %f, \"average_number_of_attempts_per_project\": %f, \"level\": %s, \"number_achievements\": %i}]}"
			, login, (int)user->id, user->full_name, user->email, user->campus_city, user->campus_country,
			data.correction_points, data.wallet, data.av_wallet_increase_per_project,
			data.num_projects, data.total_attempts, data.high_grade, data.low_grade, data.av_grade,
			data.days42, data.days_finish_project, data.hours_finish_project,
			data.attemps_per_project, data.level, data.num_achievements);
		}
		free(login);
	}
	(void) fn_data;
}