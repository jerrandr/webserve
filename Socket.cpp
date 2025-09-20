/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:21:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:26:11 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"


Socket::Socket(Pollfd *tmp, Config c,std::vector<struct addrinfo *> &struct_addr)
{
    struct addrinfo hints;
    struct addrinfo *server;
    int yes;


    yes = 1;
    polls = tmp;
    config = c;
    memset(&hints, 0, sizeof hints);
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  
    if(getaddrinfo(config.get_host().c_str(),config.get_port().c_str(),&hints,&server)!=0)
    {
        throw std::logic_error("Error getaddrinfo()");
    }
    struct_addr.push_back(server);
    fd = socket(server->ai_family,server->ai_socktype,0);
    if(fd == -1)
    {
        free_addrinfo(struct_addr);
        throw std::logic_error("Error socket()");
    }
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(yes));
    if(bind(fd,server->ai_addr,server->ai_addrlen) == -1)
    {
        close(fd);
        free_addrinfo(struct_addr);
        throw std::logic_error("Error bind()");
    }
    info = server;
}
Socket::Socket()
{
   
}
Socket::~Socket()
{
}
Socket::Socket(const Socket &other)
{
    if (this == &other)
        return ;
    *this = other;
}
Socket &Socket::operator=(const Socket &other)
{
     if (this == &other)
        return *this;
    polls = other.polls;
    fd = other.fd;
    config = other.config;
    info = other.info;
    return (*this);
}
int Socket::get_socket()const
{
    return fd;
}

void    Socket::add_new_fd()
{
    int socket_client;

    socket_client = accept(fd,NULL,NULL);
    std::perror("accept ");
    Client cl(socket_client,this->polls,this->config);
    clients.push_back(cl);
    polls->add_new_fd(socket_client);
}

void    Socket::listen_port()
{
    short re;

    re = polls->get_status(fd);
    if(re != 0 && (re & POLLIN) == 1)
        add_new_fd();
    else
    {
        re = 0;
        for(std::list<Client>::iterator j = clients.begin(); j != clients.end(); j++)
        {
            if ((*j).get_socket_client()!= -1)
            {
                
            re = polls->get_status((*j).get_socket_client());
            try
            {
                if(re)
                {
                    if (re & POLLIN)
                        (*j).verify_connex(1);
                }
                if ((*j).size_fd_wait() != 0 && (*j).get_status_request() == 1 )
                {
                    (*j).parse_request(); 
                }
                else if ((*j).get_status_request() == 1 && (*j).size_fd_wait() == 0 && (*j).get_request() != "")
                {
                    (*j).parse_request();
                }
                else if ((*j).get_status_request() != 1 && (*j).get_request() != "")
                {
                    if (time(NULL) - (*j).get_timeout() >= 60 && !(re & POLLIN))
                    {
                       (*j).exec_request_timeout();
                    }
                }
                else if( time(NULL) - (*j).get_timeout_client() >= 60 && !(re & POLLIN))
                {
                    (*j).set_status_client(-1);
                }
                if ((*j).get_status() < 0)
                {
                    std::list <Client>::iterator tmp = j;
                    polls->erase_fd((*j).get_socket_client());
                    close((*j).get_socket_client());
                    (*j).set_socket(-1);
                    j++;
                    clients.erase(tmp);
                    if (j++ == clients.end())
                        break;
                }
            }
            catch(std::bad_alloc &e)
            {
                (void)e;
                try
                {
                    (*j).exec_error_server();
                }
                catch(std::bad_alloc& e)
                {
                   (void)e;
                   (*j).exec_500();
                }
                
            }
            catch (std::out_of_range &e)
            {
                (void)e;
                try
                {
                    (*j).exec_error_server();
                }
                catch(std::out_of_range& e)
                {
                   (void)e;
                   (*j).exec_500();
                }
            }
            
        }
    }
    }
}

std::list<Client> &Socket::get_clients()
{
    return this->clients;
}
