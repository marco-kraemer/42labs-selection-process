/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   database.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 18:27:12 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/20 15:30:19 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

void	create_db(void)
{
	con = mysql_init(NULL);
	
	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	if (mysql_real_connect(con, "mysql_container", "root", DB_PASSWORD, NULL, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	query_mysql(con, "CREATE DATABASE IF NOT EXISTS api");
	query_mysql(con, "USE api");
	query_mysql(con, "DROP TABLE IF EXISTS students");
	query_mysql(con, "CREATE TABLE students(id INT, login VARCHAR(30), correction_points SMALLINT, wallet INT, num_projects SMALLINT, total_attempts SMALLINT, av_grade TINYINT, highest_grade TINYINT, lowest_grade TINYINT, days_in_42 SMALLINT, level DECIMAL(6, 2), num_achievements TINYINT);");
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

char	*get_from_db(char *column)
{
	char	buffer[1048];

	sprintf(buffer, "SELECT %s FROM students WHERE id=%i", column, (int)data->id);
	query_mysql(con, buffer);
	MYSQL_RES *result = mysql_store_result(con);
	MYSQL_ROW row = mysql_fetch_row(result);
	return (row[0]);
}
