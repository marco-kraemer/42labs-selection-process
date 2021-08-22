/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 14:34:23 by user42            #+#    #+#             */
/*   Updated: 2021/08/22 15:12:43 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

void	get_stats_info(struct Data *data)
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

void	get_user_info(struct user *user)
{
	user->login = get_from_db("user", "login");
	user->full_name = get_from_db("user", "full_name");
	user->email = get_from_db("user", "email");
	user->campus_country = get_from_db("user", "campus_country");
	user->campus_city = get_from_db("user", "campus_city");
	return ;
}
