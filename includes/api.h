/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 20:17:41 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/19 17:48:35 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	API_H
#define	API_H

#include "../mongoose.h"
#include <stdio.h>
#include <mysql/mysql.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../mjson.h"

#define ACCESS_API_INFO "grant_type=client_credentials&client_id=1a36175e33092d0d70acdf23600c9d9ceaf7fdc1146e8db6e2b609a6d90786e9&client_secret=73d987f5a96286c186f5c72f4c28393d84733bcd26a13e3679dd6b0fab4f0ba9"

struct string
{
	char	*ptr;
	size_t	len;
};

struct	json_datas
{
	struct json_object *token;

	struct json_object *parsed_jsons;
	struct json_object *parsed_json;

	struct json_object *correction_points;
	
	struct json_object *wallet;
	
	struct json_object *start_date;
	
	struct json_object *projects_users;
	struct json_object *project;
		struct json_object *project_grade;
};

struct data
{
	char	*content;

	char	*login;
	double	id;

	double	correction_point;
	double	wallet;
	char	*start_data;
	double	num_projects;
};

void		query_mysql(MYSQL *con, const char *s);

char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);

int		get_id_login(char *user);

#endif
