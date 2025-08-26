/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:14:17 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/18 07:34:17 by jerrandr         ###   ########.fr       */
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

void    WebServer::non_stop_serv()
{
    signal(SIGINT,SignalHandling::handle_signal); 
    while(1)
    {
        // print_all_fd(fds,size);
        //maj_fd();
        //maj_size_fd_socket();
        polls->start_poll();
        //poll(&fds[0],size,300);
        //maj_all_socket();
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
        std::cout << cfg[i].get_host() << std::endl;
        std::cout << cfg[i].get_port() << std::endl;
        servs.push_back(sev);

    }

    non_stop_serv();
}