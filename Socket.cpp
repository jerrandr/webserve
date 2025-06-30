/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:21:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/06/30 12:06:47 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Client.hpp"
// Socket::Socket(std::vector <pollfd> &fds, int &size, int i)
// {
//     struct addrinfo hints;
//     struct addrinfo *server;
//     int yes;

//     yes = 1;
//     fd_serv = fds;
//     size_fd = size;
//     /*initialisation apartir du fichier de config*/
//     config["listen"] = "8080";
//     config["host"] = "localhost";
//     config["root"] = "www/";
//     config["index"] = "index.html";
//     config["server_name"] = "";
//     config["def_error_page"] = "";
//     config["max_allowed_size"] = "";
    
//     /*initialisation du block location a partir du fichier de config
//     mety bedebe le location*/
//     location[0]["URI"] = "";
//     /*possible liste fa separeo espace fotsiny ex = "POST GET"*/
//     location[0]["http_method_accepted"] = "";
//     location[0]["redirect"] ="";
//     location[0]["root"] = "";
//     /*valeur booleen*/
//     location[0]["enable_director_listen"] = "";
//     location[0]["index"]="";
//     /*path*/
//     location[0]["CGI"] = "";
    
//     memset(&hints, 0, sizeof hints);
//     hints.ai_family=AF_INET;
//     hints.ai_socktype=SOCK_STREAM;
//     hints.ai_flags = AI_PASSIVE;  
//     if(getaddrinfo(config.at("host").c_str(),config.at("listen").c_str(),&hints,&server)!=0)
//     {
//         std::perror("error");
//         exit(1);
//     }
//     fd = socket(server->ai_family,server->ai_socktype,0);
//     if(fd == -1)
//     {
//         std::perror("error 1:");
//         exit(1);
//     }
//     info = server;
//     std::cout << "Server connection ......" << std::endl;
//     setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(yes));
//     if(bind(fd,server->ai_addr,server->ai_addrlen) == -1)
//     {
//         std::perror("Bind error ");
//         exit(1);
//     }
//     std::cout << "server connected" << std::endl;
//     fd_serv[i].fd=fd;
//     fd_serv[i].events = fds[i].events;
//     fd_serv[i].revents = fds[i].revents;

// }
Socket::Socket()
{
struct addrinfo hints;
    struct addrinfo *server;
    int yes;
    int size;

    yes = 1;
    size = 0;
    // fd_serv = fds;
    size_fd = 0;
    /*initialisation apartir du fichier de config*/
    config["listen"] = "8080";
    config["host"] = "localhost";
    config["root"] = "www/";
    config["index"] = "index.html";
    config["server_name"] = "";
    config["def_error_page"] = "";
    config["max_allowed_size"] = "";
    
    /*initialisation du block location a partir du fichier de config
    mety bedebe le location*/
    for(int i = 0; i < size; i++)
    {
        std::map<std::string, std::string> loc;
        
        loc["URI"] = "";
        /*possible liste fa separeo espace fotsiny ex = "POST GET"*/
        loc["http_method_accepted"] = "";
        loc["redirect"] ="";
        loc["root"] = "";
        /*valeur booleen*/
        loc["enable_director_listen"] = "";
        loc["index"]="";
        /*path*/
        loc["CGI"] = "";
    }
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  
    if(getaddrinfo(config.at("host").c_str(),config.at("listen").c_str(),&hints,&server)!=0)
    {
        std::perror("error");
        exit(1);
    }
    fd = socket(server->ai_family,server->ai_socktype,0);
    if(fd == -1)
    {
        std::perror("error 1:");
        exit(1);
    }
    info = server;
    std::cout << "Server connection ......" << std::endl;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(yes));
    if(bind(fd,server->ai_addr,server->ai_addrlen) == -1)
    {
        std::perror("Bind error ");
        exit(1);
    }
    std::cout << "server connected" << std::endl;
    // fd_serv[i].fd=fd;
    // fd_serv[i].events = fds[i].events;
    // fd_serv[i].revents = fds[i].revents;
}
Socket::~Socket()
{
    freeaddrinfo(info);
}
Socket::Socket(const Socket &other)
{
    *this = other;
}
Socket &Socket::operator=(const Socket &other)
{
    fd_serv.clear();
    fd_serv = other.fd_serv;
    fd = other.fd;
    size_fd  = other.size_fd;
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
    if(socket_client == -1)
    {
        std::perror("Error :");
        exit(1);
    }
    Client cl(socket_client,"");
    clients.push_back(cl);
}
void    Socket::set_size_fd(int size)
{
    this->size_fd = size;
}
void    Socket::set_poll(std::vector <pollfd> &fds)
{
    fd_serv.clear();
    fd_serv = fds;
}

void    Socket::maj_fd_client()
{
    size_t size;

    size = 0;

    while(true)
    {
        for(std::vector<Client>::iterator i= clients.begin(); i != clients.end(); i++)
        {
            size ++;
            if(i->get_status() < 0)
            {
                close(i->get_socket_client());
                clients.erase(i);
                size = 0;
                break;
            }
        }
        if(size == clients.size())
            break;   
    }
}
void    Socket::listen_port()
{
    for(int i = 0; i < size_fd; i++)
    {
        if(fd_serv[i].fd == fd && (fd_serv[i].revents & POLLIN) == 1)
        {
            add_new_fd();
        }
        else if(fd_serv[i].revents > 0)
        {
            for(size_t j = 0; j < clients.size(); j++)
            {
                if(fd_serv[i].fd == clients[j].get_socket_client())
                {
                    if (fd_serv[i].revents & POLLIN)
                        clients[j].verify_connex(1);
                    else
                        clients[j].verify_connex(2);
                }
            }
        }
    }
    maj_fd_client();
}

std::vector<Client> &Socket::get_clients()
{
    return this->clients;
}
