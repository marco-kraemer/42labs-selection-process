/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 20:17:41 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 15:14:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	API_H
#define	API_H

#include "../libraries/mongoose.h"
#include <stdio.h>
#include <mysql/mysql.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../libraries/mjson.h"
#include <time.h>

#define ACCESS_API_INFO "grant_type=client_credentials&client_id=1a36175e33092d0d70acdf23600c9d9ceaf7fdc1146e8db6e2b609a6d90786e9&client_secret=73d987f5a96286c186f5c72f4c28393d84733bcd26a13e3679dd6b0fab4f0ba9"
#define DB_PASSWORD "password"

struct Data
{
	int	correction_points;
	int	wallet;
	float	av_wallet_increase_per_project;
	int	num_projects;
	int	total_attempts;
	int	av_grade;
	int	high_grade;
	int	low_grade;
	int	days42;
	float	days_finish_project;
	float	hours_finish_project;
	float	attemps_per_project;
	char	*level;
	int	num_achievements;
};

struct user
{
	char	*login;
	double	id;
	char	*full_name;
	char	*email;
	char	*campus_country;
	char	*campus_city;
};

struct date
{
	int	day;
	int	month;
	int	year;
};


struct Date2
{
	int	d, m ,y;
};

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
	struct json_object *updated_date;

	struct json_object *projects_users;
	struct json_object *project;
		struct json_object *project_grade;
		struct json_object *occurrence;
	
	struct json_object *cursus_users;
		struct json_object *level;

	struct json_object	*achievements;

	struct json_object *login;
	struct json_object *full_name;
	struct json_object *email;
	struct json_object *campus;
		struct json_object *country;
		struct json_object *city;
};


void	server(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
int	getDifferenceDates(struct Date2 dt1, struct Date2 dt2);

void		query_mysql(MYSQL *con, const char *s);
void		create_db(void);
char		*get_from_db(char *column, char *table);

int		update_db(char *user);

char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);

int		get_id_login(char *user);
int		get_token(void);
void		get_user_info(struct user *user);
void		get_stats_info(struct Data *data);

size_t 		writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

void	init_string(struct string *s);

extern MYSQL 	*con;
extern char	*mytoken;
extern double	id;
#endif
