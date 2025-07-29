/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:57:28 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/28 14:53:00 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "utils.h"
struct pollfd;
class Socket;
class Server
{
    private:
        Config config;
        std::vector<Socket> sockets;
        std::vector<int> serv;
        int size;
        Pollfd *fds;
        // std::vector <pollfd> fds;
        // struct pollfd fds[10000];
        Server();
    public:
        Server(Config c, Pollfd *p);
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
                void    free_all_socket_info();
};
#endif