/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 11:04:27 by user42            #+#    #+#             */
/*   Updated: 2021/08/21 11:25:37 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

void server(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
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