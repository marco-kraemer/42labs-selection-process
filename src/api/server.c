/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 11:04:27 by user42            #+#    #+#             */
/*   Updated: 2021/08/22 15:23:46 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/api.h"

void server(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	struct Data	data;
	struct user	user;

	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		char	*login = ft_substr(hm->uri.ptr, 1, hm->uri.len - 1);
		get_id_login(login);
		if (id == -1)
			mg_http_reply(c, 404,"Content-Type: application/json\r\n", "{\"error\": 404, \"message\": \"student_%s_not_found\"}", login);
		else
		{
			update_db(login);
			get_user_info(&user);
			get_stats_info(&data);
			mg_http_reply(c, 200, "Content-Type: application/json\r\n",
			"{\"user\": [{\"login\": \"%s\", \"id\": %i, \"full_name\": \"%s\", \"email\": \"%s\", \"campus_city\": \"%s\", \"campus_country\": \"%s\"}],\"stats\":[{\"correction_points\": %i, \"wallet\": %i, \"average_wallet_increase_per_project\": %f, \"number_of_projects_done\": %i, \"number_of_attempts\":%i, \"highest_grade\": %i, \"lowest_grade\": %i, \"average_grade\": %i, \"days_in_42\":%i, \"days_finish_a_project\": %f, \"hours_finish_a_project\": %f, \"average_number_of_attempts_per_project\": %f, \"level\": %s, \"number_achievements\": %i}]}"
			, login, (int)id, user.full_name, user.email, user.campus_city, user.campus_country,
			data.correction_points, data.wallet, data.av_wallet_increase_per_project,
			data.num_projects, data.total_attempts, data.high_grade, data.low_grade, data.av_grade,
			data.days42, data.days_finish_project, data.hours_finish_project,
			data.attemps_per_project, data.level, data.num_achievements);
		}
		free(login);
	}
	(void) fn_data;
}