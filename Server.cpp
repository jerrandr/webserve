/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:05:17 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/23 10:32:26 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Socket.hpp"
#include "Client.hpp"
Server::Server()
{

    // /*io size io ny isanle block server*/
    // size = 1;
    // config = -1;
    // fds = new Pollfd();
    // if(fds == NULL)
    //     return ;
    // for(int i = 0; i < size; i++)
    // {
    //     Socket socket_serv(fds,);
    //     sockets.push_back(socket_serv);
    //     serv.push_back(socket_serv.get_socket());
    //     fds->add_new_fd(socket_serv.get_socket());
    // }  
}

Server::Server(Config c, Pollfd *p,std::vector<struct addrinfo *> &struct_addr)
{
    std::vector<std::string> ports;
    std::vector<std::string> hosts;
    /*io size io ny isanle socket*/
    ports = split(c.get_port()," ");
    hosts = split(c.get_host()," ");
    if (hosts.size() != ports.size())
        throw std::logic_error("configuration error1");
    size = ports.size();
    // config = -1;
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



// void    Server::maj_fd()
// {
//     struct pollfd fd_poll;

//     fds.clear();
//     for(size_t i = 0;i < serv.size() ; i++)
//     {
//         fd_poll.fd = serv[i];
//         fd_poll.events = POLLIN;
//         fd_poll.revents = 0;
//         fds.push_back(fd_poll);
//     }
//     for(size_t i = 0 ; i < sockets.size(); i++)
//     {
//         std::vector<Client> tmp;
//         tmp = sockets[i].get_clients();
//         for(size_t j = 0; j < tmp.size(); j++)
//         {
//             fd_poll.fd = tmp[j].get_socket_client();
//             fd_poll.events = POLLIN | POLLOUT;
//             fd_poll.revents = 0;

//             fds.push_back(fd_poll);
//         }
//         tmp.clear();
//     }
//     size = (int)fds.size();
// }
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
        listen(serv[i],1000);
    }       
}

// void    Server::maj_all_socket()
// {
//     for(size_t i = 0; i < sockets.size(); i++)
//         sockets[i].set_poll(fds);
// }

void    print_all_fd(struct pollfd (fd_serv)[10000],int size)
{
    std::cout << "----------------------------" << std::endl;
    for(int i =0; i < size;i++)
    {
        std::cout << " " << fd_serv[i].fd;
    }
    std::cout <<std::endl<< "----------------------------" << std::endl;
}

// void    Server::maj_size_fd_socket()
// {
//     for(size_t i =0; i < sockets.size(); i++)
//     {
//         sockets[i].set_size_fd(size);
//     }
// }
void    Server::start()
{
    // listen_all_socket();
    // signal(SIGINT,SignalHandling::handle_signal); 
    // while(1)
    // {
        // print_all_fd(fds,size);
        //maj_fd();
        //maj_size_fd_socket();
        // fds->start_poll();
        //poll(&fds[0],size,300);
        //maj_all_socket();
        listen_sockets();
    // }
}

// void    Server::free_all_socket_info()
// {
//     for(std::size_t i = 0; i < sockets.size(); i++)
//     {
//             sockets[i].free_addrinfo();
//     }
// }