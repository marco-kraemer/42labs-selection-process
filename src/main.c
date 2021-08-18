/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraurel <maraurel@student.42sp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 15:53:11 by maraurel          #+#    #+#             */
/*   Updated: 2021/08/18 00:30:52 by maraurel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/api.h"

static const char *s_http_addr = "http://0.0.0.0:3000";
static const char *s_root_dir = ".";

struct data
{
	char	*content;
	char	*token;
};

struct data *data;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		if (mg_http_match_uri(hm, "/helloworld"))
		{
			mg_http_reply(c, 200, "", "Hello World");  // Serve REST
		}
		else
		{
			mg_http_reply(c, 404, "", "Student not found\n", (int) hm->uri.len, hm->uri.ptr);
		}
		
	}
	(void) fn_data;
	(void) s_root_dir;
}

static size_t	writecallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	data = (struct data *) userp;

	data->content = contents;
	return realsize;

}

int	get_informations(void)
{
	CURL *curl = curl_easy_init();
	if(curl)
	{
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.intra.42.fr/oauth/token");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ACCESS_API_INFO);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
		// Perform
		res = curl_easy_perform(curl);
		// Check error
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform failed\n");
			return (1);
		}
		curl_easy_cleanup(curl);
		return (0);
	}
	return (1);
}

int main(void)
{
	struct json_object *parsed_json;
	struct json_object *token;

	get_informations();
	parsed_json = json_tokener_parse(data->content);
	json_object_object_get_ex(parsed_json, "access_token", &token);
	printf("TOKEN: %s\n", json_object_get_string(token));
	// Event manager
	struct mg_mgr	mgr;
	// Initialise event manager
	mg_mgr_init(&mgr);
	// Create HTTP listener
	mg_http_listen(&mgr, s_http_addr, fn, NULL);
	// Infinite event loop
	for (;;)
		mg_mgr_poll(&mgr, 1000);
	// Free allocated memory
	mg_mgr_free(&mgr);
	return 0;
}
