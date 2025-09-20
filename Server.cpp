/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:05:17 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:25:02 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Socket.hpp"
#include "Client.hpp"
Server::Server()
{

}

Server::Server(Config c, Pollfd *p,std::vector<struct addrinfo *> &struct_addr)
{
    std::vector<std::string> ports;
    std::vector<std::string> hosts;
    ports = split(c.get_port()," ");
    hosts = split(c.get_host()," ");
    if (hosts.size() != ports.size())
        throw std::logic_error("configuration error1");
    size = ports.size();
    fds = p;
    config = c;
    if(fds == NULL)
        return ;
    for(int i = 0; i < size; i++)
    {
        c.set_host(hosts[i]);
        c.set_port(ports[i]);
        Socket socket_serv(p, c,struct_addr);
        sockets.push_back(socket_serv);
        serv.push_back(socket_serv.get_socket());
        fds->add_new_fd(socket_serv.get_socket());
    } 
     listen_all_socket(); 
}
Server::~Server()
{
    
    
}
Server::Server(const Server &other)
{
    *this = other;
}
Server &Server::operator=(const Server &other)
{
    config = other.config;
    sockets = other.sockets;
    serv = other.serv;
    size = other.size;
    fds = other.fds;
    return (*this);
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
        listen(serv[i],INT_MAX);
    }       
}


void    Server::start()
{
        listen_sockets();
}
