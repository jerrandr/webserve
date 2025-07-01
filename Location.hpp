/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:14:00 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 11:17:22 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
class Location
{
    private:
        std::string URI;
        /*possible liste fa separeo espace fotsiny ex = "POST GET"*/
        std::string http_method_accepted;
        std::string redirect;
        std::string root;
        bool enable_director_listen;
        /*possible liste fa separeo espace koa*/
        std::string index;
        /*PATH*/
        std::string CGI;
    public:
        Location();
        ~Location();
        Location(const Location &other);
        Location &operator=(const Location &other);

        /*getters*/
        const std::string get_uri() const;
        const std::string get_meth() const;
        const std::string get_redir() const;
        const std::string get_root() const;
        const std::string get_index() const;
        const std::string get_path_cgi() const;
        bool    get_directory_listen() const;


        /*setters*/
        void    set_uri(const std::string &uri);
        void    set_meth(const std::string &meth);
        void    set_redir(const std::string &redir);
        void    set_root(const std::string &roots);
        void    set_enabled(const bool &enabled);
        void    set_index(const std::string &i);
        void    set_path_cgi(const std::string &path);
};


#endif