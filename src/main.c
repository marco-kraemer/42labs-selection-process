/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 09:47:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

MYSQL 			*con;
char			*mytoken;
struct user 		*user;

int	main(void)
{
	printf("Starting...\n");
	create_db(); // Create database
	get_token(); // Get Token
	struct mg_mgr	mgr; // Event manager
	mg_mgr_init(&mgr); // Initialise event manager
	mg_http_listen(&mgr, "http://0.0.0.0:3000", server, NULL); // Create HTTP listener
	for (;;)
		mg_mgr_poll(&mgr, 1000); // Infinite event loop
	free(mytoken);
	mg_mgr_free(&mgr);
	return (0);
}
