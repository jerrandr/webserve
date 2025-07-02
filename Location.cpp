/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:23:00 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/02 09:56:10 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
    URI = "";
    http_method_accepted = "";
    redirect = "";
    root = "";
    enable_director_listen = false;
    index = "";
    CGI = "";
    script_cgi = "";
}
Location::~Location()
{
    
}
Location::Location(const Location &other)
{
    *this = other;
}
Location &Location::operator=(const Location &other)
{
    URI = other.URI;
    http_method_accepted = other.http_method_accepted;
    redirect = other.redirect;
    root = other.root;
    enable_director_listen = other.enable_director_listen;
    index = other.index;
    CGI = other.CGI;
    script_cgi = other.script_cgi;
    return *this;
}

/*getters*/
const std::string Location::get_uri() const
{
    return URI;
}
const std::string Location::get_meth() const
{
    return http_method_accepted;
}
const std::string Location::get_redir() const
{
    return redirect;
}
const std::string Location::get_root() const
{
    return root;
}
const std::string Location::get_index() const
{
    return index;
}
const std::string Location::get_path_cgi() const
{
    return CGI;
}
bool    Location::get_directory_listen() const
{
    return enable_director_listen;
}


/*setters*/
void    Location::set_uri(const std::string &uri)
{
    URI = uri;
}
void    Location::set_meth(const std::string &meth)
{
    http_method_accepted = meth;
}
void    Location::set_redir(const std::string &redir)
{
    redirect = redir;
}
void    Location::set_root(const std::string &roots)
{
    root = roots;
}
void    Location::set_enabled(const bool &enabled)
{
    enable_director_listen = enabled;
}
void    Location::set_index(const std::string &i)
{
    index = i;
}
void    Location::set_path_cgi(const std::string &path)
{
    CGI = path;
}
const std::string Location::get_script_cgi() const
{
    return script_cgi;
}
void    Location::set_script(const std::string &src)
{
    script_cgi = src;
}