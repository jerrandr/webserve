/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:50:04 by msalohy           #+#    #+#             */
/*   Updated: 2025/10/15 10:19:41 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP
#include "utils.h"
class Server;
class WebServer
{
    private:
        Pollfd *polls;
        std::vector<Server> servs;
        void    no_stop_serv();
        std::vector<struct addrinfo *> struct_addr;
    public:
        WebServer();
        ~WebServer();
        WebServer(const WebServer &other);
        WebServer &operator=(const WebServer &other);
        void start_webserver(int fd);
        void    add_new_addrinfo(std::vector<struct addrinfo *> i);
};
#endif