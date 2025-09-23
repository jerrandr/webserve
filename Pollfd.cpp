/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pollfd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:19:58 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/23 11:53:48 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pollfd.hpp"

Pollfd::Pollfd()
{
    new_fd = 0;
}
Pollfd::~Pollfd()
{

}
Pollfd::Pollfd(const Pollfd &other)
{
    *this = other;
}
Pollfd &Pollfd::operator=(const Pollfd &other)
{
    fds = other.fds;
    tab_fds = other.tab_fds;
    return *this;
}

bool    Pollfd::fd_is_here(int fd)
{
     try
    {
        (void)tab_fds.at(fd);
        return true;
    }
    catch(const std::out_of_range &e)
    {
        (void)e;
    }
    
    return false;
}
void    Pollfd::add_new_fd(int fd)
{
    struct pollfd tmp;

    tmp.fd = fd;
    tmp.events = POLLIN|POLLOUT;
    tmp.revents = 0;
    tab_fds[fd] = 0;
    fds.push_back(tmp);
}

void    Pollfd::set_new_fd()
{
    std::cout << "increase" << std::endl;
    new_fd += 1;
}
int     Pollfd::get_new_fd_poll()
{
    return new_fd;
}

void    Pollfd::decrement_new_fd()
{
    new_fd -= 1;
    std::cout << "decrease" << std::endl;
}
void    Pollfd::start_poll()
{
    if(fds.size() <= 0)
        return ;
    if (poll(&(fds[0]),(int)fds.size(),-1) == -1)
    {
        return;
    }
    for(std::vector<pollfd>::iterator i = fds.begin(); i != fds.end();i++)
    {
        tab_fds[(*i).fd] = (*i).revents;
    }
}
short   Pollfd::get_status(int fd) 
{
    try
    {
        (void)tab_fds.at(fd);
        return tab_fds.at(fd);
    }
    catch(std::out_of_range &e)
    {
        (void)e;
        return -1;
    }
}

int Pollfd::get_size()
{
    return (int)fds.size();
}

void    Pollfd::erase_fd(int fd)
{
    try
    {
        (void)tab_fds.at(fd);
        for(std::vector<pollfd>::iterator i = fds.begin(); i != fds.end();i++)
        {
            if((*i).fd == fd)
            {
                close(fd);
                fds.erase(i);
                break;
            }
        }
        tab_fds.erase(fd);
    }
    catch(std::out_of_range &e)
    {
        (void)e;
    }
}

void    Pollfd::close_all_socket()
{
        for(std::vector<pollfd>::iterator i = fds.begin(); i != fds.end();i++)
        {
            close((*i).fd);
        }
}