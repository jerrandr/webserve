/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:50:04 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/28 14:52:57 by msalohy          ###   ########.fr       */
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
        void    non_stop_serv();
    public:
        WebServer();
        ~WebServer();
        WebServer(const WebServer &other);
        WebServer &operator=(const WebServer &other);
        void start_webserver(int fd);
};
#endif