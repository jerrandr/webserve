/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pollfd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:14:57 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/23 10:04:37 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLFD_HPP
#define POLLFD_HPP
#include <poll.h>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <unistd.h>
struct pollfd;
class Pollfd
{
    private:
        std::vector<pollfd> fds;
        std::map<int,short> tab_fds;
        int new_fd;
    public:
        Pollfd();
        ~Pollfd();
        Pollfd(const Pollfd &other);
        Pollfd &operator=(const Pollfd &other);
        
        void    add_new_fd(int fd);
        void    start_poll();
        short   get_status(int fd);
        int get_size();
        void    erase_fd(int fd);
        bool    fd_is_here(int fd);
        void    set_new_fd();
        int     get_new_fd_poll();
        void    decrement_new_fd();

        void    close_all_socket();
};


#endif