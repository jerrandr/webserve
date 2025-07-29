/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:24:34 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/29 13:48:34 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "utils.h"
class Config
{
    private:
        std::string listen;
        std::string host;
        std::string max_allowed_size;
        std::vector<Location> locs;
        ErrorPage errors;
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
        std::size_t get_max_allowed_size() const;
        const std::vector<Location> &get_locs() const;
        const ErrorPage &get_errors() const;
        std::string get_mime(const std::string &type);
        
        /*setters*/
        void    set_port(const std::string &port);
        void    set_host(const std::string &h);
        void    set_max_allowed_size(const std::string &max);
        void    set_locs(const Location &loc);
        void    set_errors(const ErrorPage &err);

        /*other get*/
        Location    get_location_match(std::string uri);
        std::string get_real_path(std::string uri, Location loc);

};

#endif