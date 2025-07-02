/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:57:28 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 13:35:39 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "Location.hpp"
#include "ErrorPage.hpp"
#include "Config.hpp"
#include "Pollfd.hpp"
struct pollfd;
class Socket;
class Server
{
    public:
        int config;
        std::vector<Socket> sockets;
        std::vector<int> serv;
        int size;
        Pollfd *fds;
        // std::vector <pollfd> fds;
        // struct pollfd fds[10000];
    public:
        Server();
        ~Server();
        Server(const Server &other);
        Server &operator=(const Server &other);

        Server(std::string name);
        void    start();
        void    maj_fd();
        void    listen_sockets();
        void    listen_all_socket();
        void    maj_all_socket();

        void    maj_size_fd_socket();
};
#endif