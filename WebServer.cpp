/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:14:17 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/29 09:24:21 by msalohy          ###   ########.fr       */
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
    for(std::size_t i = 0; i < servs.size();i++)
        servs[i].free_all_socket_info();
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
    /*initialisation de tous les socket selon le fichier de config venant du fd*/
    config_parsing(fd);
    int size = 1;

    for(int i = 0; i < size; i++)
    {
        Config config;
        
        config.set_port("8080");
        config.set_host("localhost");
        // config.set_max_allowed_size("");
    
        /*initialisation du block location a partir du fichier de config
        mety bedebe le location*/
        for(int i = 0; i < size; i++)
        {
            Location loc;

                loc.set_uri("/");
        /*possible liste fa separeo espace fotsiny ex = "POST GET"*/
        loc.set_meth("POST GET");
        loc.set_redir("");
        loc.set_root("");
        /*valeur booleen*/
        loc.set_enabled(true);
        loc.set_index("");
        /*path*/
        loc.set_path_cgi("");
        config.set_locs(loc);
        }
    
        /*raha misy directive error page ao amn fichier de config de atao otranio location*/
        Server sev(config, polls);
        servs.push_back(sev);
    }

    non_stop_serv();
}