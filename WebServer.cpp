/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:14:17 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:36:59 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "Server.hpp"

WebServer::WebServer()
{
    polls = NULL;
}
WebServer::~WebServer()
{
    free_addrinfo(struct_addr);
    if (polls)
    {
        polls->close_all_socket();
        delete polls;
        polls = NULL;
    }
}
WebServer::WebServer(const WebServer &other)
{
    (void)other;
}
WebServer &WebServer::operator=(const WebServer &other)
{
    (void)other;
    return (*this);
}

void    WebServer::no_stop_serv()
{
    signal(SIGINT,SignalHandling::handle_signal); 
    while(1)
    {
        polls->start_poll();
        for(std::size_t i = 0; i < servs.size();i++)
            servs[i].start();
    }
}
void WebServer::start_webserver(int fd)
{

    polls = new Pollfd();
    std::vector<Config> cfg;

    config_parsing(fd, cfg);

    for(size_t i = 0; i < cfg.size(); i++)
    {
        Server sev(cfg[i], polls,struct_addr);
        servs.push_back(sev);

    }

    no_stop_serv();
}