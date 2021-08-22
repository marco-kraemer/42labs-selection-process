/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/22 15:23:43 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/api.h"

MYSQL 			*con;
char			*mytoken;
double			id;

int	main(void)
{
	struct mg_mgr	mgr;

	printf("Starting...\n");
	create_db();
	get_token();
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "http://0.0.0.0:3000", server, NULL);
	for (;;)
		mg_mgr_poll(&mgr, 1000);
	free(mytoken);
	mg_mgr_free(&mgr);
	return (0);
}
