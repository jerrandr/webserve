/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:35 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/13 13:54:23 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

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
#include "NotReady.hpp"
std::vector<std::string> split(std::string str, std::string sep);
std::string get_html_page(int fd);
int fd_is_ready(std::string path, Pollfd *polls, std::map<std::string, int> &fd_wait);
void  fd_closed(int fd,Pollfd *polls, std::map<std::string, int> &fd_wait,std::string path);

bool is_directory(std::string path);
std::string decode_str(std::string d);
void    config_parsing(int fd, std::vector<Config> &cfg);
int     address_check(std::string add, Config &cfg, std::vector<std::string> &key_vect);
int     port_check(std::string port, Config &cfg, std::vector<std::string> &key_vect);
int     error_page_set(std::string path, ErrorPage &err_page, std::vector<std::string> &err_vect);
int     error_page_occurences(std::vector<std::string> &error_vect);
int     get_principal_uri(std::string value, Config &cfg);
int     method_check(std::string value, std::vector<std::string> &found_key);
int     root_check(std::string value, std::vector<std::string> &found_key);
int     autoindex_check(std::string value, Location &lcs, std::vector<std::string> &found_key);
int     index_check(std::string value, std::vector<std::string> &found_key);
int     upload_check(std::string value, std::vector<std::string> &found_key);
int     cgi_check(std::string value, std::vector<std::string> &found_key);
int     redirect_check(std::string value, std::vector<std::string> &found_key);
int     cgi_path_check(std::string value, std::vector<std::string> &found_key);
int     check_port_and_host(Config &cfg);
int     multiple_key_check(std::vector<std::string> &found_key);
void    check_minimum_value(std::vector<Config> &cfg, int fd);
int     check_all_error_path(Config &cfg);
int     brakes_check(std::string all_string);
int     incrementation(std::string line);
int     check_bloc_value(std::string value);
std::string     reform_value(std::string  str);


void    free_addrinfo(std::vector<struct addrinfo *> &struct_addr);

std::string get_mime_type(std::string type);
int body_chunked(int len1, std::string body);
std::vector<std::string> split_sep(std::string input, std::string sep);
#endif