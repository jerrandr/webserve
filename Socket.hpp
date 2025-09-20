/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:15:04 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:26:26 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "Server.hpp"
#include "Client.hpp"
class Client;
class Socket
{
    private:
        Config config;
        struct addrinfo *info;
        Pollfd *polls;

        int fd;
        std::list<Client> clients;
        Socket();

    public:
        Socket(Pollfd *tmp, Config c,std::vector<struct addrinfo *> &struct_addr);

        ~Socket();
        Socket(const Socket &other);
        Socket &operator=(const Socket &other);

        int get_socket()const;

        void    listen_port();

        
        void    add_new_fd();

        std::list<Client> &get_clients();
};
#endif