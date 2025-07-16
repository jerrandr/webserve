/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:24:34 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/16 09:47:53 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <iostream>
#include <vector>
#include "Location.hpp"
#include "ErrorPage.hpp"
#include "Location.hpp"
#include "ErrorPage.hpp"
#include <map>
class Config
{
    private:
        std::string listen;
        std::string host;
        std::string server_name;
        std::string max_allowed_size;
        std::vector<Location> locs;
        std::vector<ErrorPage> errors;
        /*cles extension avec*/
        std::map<std::string,std::string> mime;
    public:
        Config();
        ~Config();
        Config(const Config &other);
        Config &operator=(const Config &other);

        /*getters*/
        const std::string &get_port() const;
        const std::string &get_host() const;
        const std::string &get_server_name() const;
        const std::string &get_max_allowed_size() const;
        const std::vector<Location> &get_locs() const;
        const std::vector<ErrorPage> &get_errors() const;
        std::string get_mime(const std::string &type);
        
        /*setters*/
        void    set_port(const std::string &port);
        void    set_host(const std::string &h);
        void    set_server_name(const std::string &name);
        void    set_max_allowed_size(const std::string &max);
        void    set_locs(const Location &loc);
        void    set_errors(const ErrorPage &err);

};

#endif