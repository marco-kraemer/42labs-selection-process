/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/21 11:10:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

const char *s_http_addr = "http://0.0.0.0:3000";
const char *s_root_dir = ".";
struct json_datas	json;
struct data 		*data;
MYSQL 			*con;
char			*mytoken;

int	main(void)
{
	printf("Starting...\n");
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
