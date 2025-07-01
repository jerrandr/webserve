/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:04:49 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 11:15:33 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
    listen = "";
    host = "";
    server_name = "";
    max_allowed_size = "";
}
Config::~Config()
{
    
}
Config::Config(const Config &other)
{
    *this = other;
}
Config &Config::operator=(const Config &other)
{
    listen = other.listen;
    host = other.host;
    server_name = other.server_name;
    max_allowed_size = other.max_allowed_size;
    locs.clear();
    locs = other.locs;
    errors.clear();
    errors = other.errors;
    return *this;
}

        /*getters*/
const std::string &Config::get_port() const
{
    return listen;
}
const std::string &Config::get_host() const
{
    return host;
}
const std::string &Config::get_server_name() const
{
    return server_name;
}
const std::string &Config::get_max_allowed_size() const
{
    return max_allowed_size;
}
const std::vector<Location> &Config::get_locs() const
{
    return locs;
}
const std::vector<ErrorPage> &Config::get_errors() const
{
    return errors;
}

/*setters*/
void    Config::set_port(const std::string &port)
{
    listen = port;
}
void    Config::set_host(const std::string &h)
{
    host = h;
}
void    Config::set_server_name(const std::string &name)
{
    server_name = name;
}
void    Config::set_max_allowed_size(const std::string &max)
{
    max_allowed_size = max;
}
void    Config::set_locs(const Location &loc)
{
    locs.push_back(loc);
}
void    Config::set_errors(const ErrorPage &err)
{
    errors.push_back(err);
}