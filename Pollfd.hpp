/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pollfd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:14:57 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 14:43:53 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLFD_HPP
#define POLLFD_HPP
#include <poll.h>
#include <vector>
#include <map>
#include <list>
#include <iostream>
struct pollfd;
class Pollfd
{
    private:
        std::vector<pollfd> fds;
        std::map<int,short> tab_fds;
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
};


#endif