/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:35 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/28 14:01:53 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

typedef struct OneServerConfigurationFile
{
    std::string     host;
    std::string     listen;
    std::string     error_page;
    std::string     cmbs;
    std::string     local;
    std::string     method;
    std::string     root;
    std::string     autoindex;
    std::string     index;
    std::string     route;
    std::string     upload;
    std::string     cgi;
}   OscF;

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <fcntl.h>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "signal.h"
#include "Location.hpp"
#include "ErrorPage.hpp"
#include "Config.hpp"
#include "Pollfd.hpp"
#include "signal_handling/SignalHandling.hpp"
std::vector<std::string> split(std::string str, std::string sep);
std::string get_html_page(int fd);
int fd_is_ready(std::string path, Pollfd *polls, std::map<std::string, int> &fd_wait);
void  fd_closed(int fd,Pollfd *polls, std::map<std::string, int> &fd_wait,std::string path);

bool is_directory(std::string path);
std::string decode_str(std::string d);
void    config_parsing(int fd);

#endif