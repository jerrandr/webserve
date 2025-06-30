/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:05:17 by msalohy           #+#    #+#             */
/*   Updated: 2025/06/24 08:35:08 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Socket.hpp"
#include "Client.hpp"
Server::Server()
{
    size = 1;
    config = -1;
    bzero(fds,10000);
    for(int i = 0; i < size; i++)
    {
        Socket socket_serv(fds,size,i);
        sockets.push_back(socket_serv);
        serv.push_back(socket_serv.get_socket());
        fds[i].fd = socket_serv.get_socket();
        fds[i].events = POLLIN;
        fds[i].revents = 0;
    }  
}
Server::~Server()
{
    std::cout << "server shut down-----------" << std::endl;
}
Server::Server(const Server &other)
{
    (void)other;
}
Server &Server::operator=(const Server &other)
{
    (void)other;
    return (*this);
}

Server::Server(std::string name)
{
    (void)name;
}

void    Server::maj_fd()
{
    int len;

    len = 0;
    for(size_t i = 0;i < serv.size() ; i++)
    {
        fds[i].fd = serv[i];
        fds[i].events = POLLIN;
        fds[i].revents = 0;
        len ++;
    }
    for(size_t i = 0 ; i < sockets.size(); i++)
    {
        std::vector<Client> tmp;
        tmp = sockets[i].get_clients();
        for(size_t j = 0; j < tmp.size(); j++)
        {
            fds[len].fd = tmp[j].get_socket_client();
            fds[len].events = POLLIN | POLLOUT;
            fds[len].revents = 0;
            len ++; 
        }
        tmp.clear();
    }
    size = len;
}
void    Server::listen_sockets()
{
    for(size_t i = 0 ; i < sockets.size(); i++)
    {
        sockets[i].listen_port();
    }
}

void    Server::listen_all_socket()
{
    for(size_t i = 0; i < serv.size();i++)
    {
        listen(serv[i],10);
    }       
}

void    Server::maj_all_socket()
{
    for(size_t i = 0; i < sockets.size(); i++)
        sockets[i].set_poll(fds);
}

void    print_all_fd(struct pollfd (fd_serv)[10000],int size)
{
    std::cout << "----------------------------" << std::endl;
    for(int i =0; i < size;i++)
    {
        std::cout << " " << fd_serv[i].fd;
    }
    std::cout <<std::endl<< "----------------------------" << std::endl;
}

void    Server::maj_size_fd_socket()
{
    for(size_t i =0; i < sockets.size(); i++)
    {
        sockets[i].set_size_fd(size);
    }
}
void    Server::start()
{
    while(1)
    {
        // print_all_fd(fds,size);
        maj_fd();
        maj_size_fd_socket();
        listen_all_socket();
        poll(fds,size,300);
        maj_all_socket();
        listen_sockets();
    }
}