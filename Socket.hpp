/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:15:04 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 14:37:16 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "Server.hpp"

class Client;
class Socket
{
    private:
        Config config;
        struct addrinfo *info;
        Pollfd *polls;
        // std::vector <pollfd> fd_serv;
        // struct pollfd (fd_serv)[10000];
        // int size_fd;
        int fd;
        std::vector<Client> clients;
        Socket();

    public:
        Socket(Pollfd *tmp);
        // Socket(std::map<std::string, std::string> config,std::vector < std::map< std::string, std::string> > location);
        // Socket(std::vector <pollfd> &fds, int &size, int i);
        ~Socket();
        Socket(const Socket &other);
        Socket &operator=(const Socket &other);

        int get_socket()const;
        // void    set_size_fd(int size);
        void    listen_port();
        void    set_poll(std::vector <pollfd> &fds);
        void    maj_fd_client();
        
        void    add_new_fd();

        std::vector<Client> &get_clients();
};
#endif