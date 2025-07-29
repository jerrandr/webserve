/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:04:49 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/29 07:09:32 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
    listen = "";
    host = "";
    server_name = "";
    max_allowed_size = "1m";

    mime[".aac"] = "audio/aac";
    mime[".abw	"] = "application/x-abiword";
    mime[".arc"] = "application/octet-stream";
    mime[".avi"] = "video/x-msvideo";
    mime[".azw"] = "application/vnd.amazon.ebook";
    mime[".bin"] = "application/octet-stream";
    mime[".bmp"] = "image/bmp";
    mime[".bz"] = "application/x-bzip";
    mime[".bz2"] = "application/x-bzip2";
    mime[".csh"] = "application/x-csh";
    mime[".css"] = "text/css";
    mime[".csv"] = "text/csv";
    mime[".doc"] = "application/msword";
    mime[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mime[".eot"] = "application/vnd.ms-fontobject";
    mime[".epub"] = "application/epub+zip";
    mime[".gif"] = "image/gif";
    mime[".htm"] = "text/html";
    mime[".html"] = "text/html";
    mime[".ico"] = "image/x-icon";
    mime[".ics"] = "text/calendar";
    mime[".jar"] = "application/java-archive";
    mime[".jpeg"] = "image/jpeg";
    mime[".jpg"] = "image/jpeg";
    mime[".js"] = "application/javascript";
    mime[".json"] = "application/json";
    mime[".mid"] = "audio/midi";
    mime[".midi"] = "audio/midi";
    mime[".mpeg"] = "video/mpeg";
    mime[".mpkg"] = "application/vnd.apple.installer+xml";
    mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
    mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    mime[".odt"] = "application/vnd.oasis.opendocument.text";
    mime[".oga"] = "audio/ogg";
    mime[".ogv"] = "video/ogg";
    mime[".ogx"] = "application/ogg";
    mime[".otf"] = "font/otf";
    mime[".png"] = "image/png";
    mime[".pdf"] = "application/pdf";
    mime[".ppt"] = "application/vnd.ms-powerpoint";
    mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mime[".rar"] = "application/x-rar-compressed";
    mime[".rtf"] = "application/rtf";
    mime[".sh"] = "application/x-sh";
    mime[".svg"] = "image/svg+xml";
    mime[".swf"] = "application/x-shockwave-flash";
    mime[".tar"] = "application/x-tar";
    mime[".tif"] = "image/tiff";
    mime[".tiff"] = "image/tiff";
    mime[".ts"] = "application/typescript";
    mime[".ttf"] = "font/ttf";
    mime[".vsd"] = "application/vnd.visio";
    mime[".wav"] = "audio/x-wav";
    mime[".weba"] = "audio/webm";
    mime[".webm"] = "video/webm";
    mime[".webp"] = "image/webp";
    mime[".woff"] = "font/woff";
    mime[".woff2"] = "font/woff2";
    mime[".xhtml"] = "application/xhtml+xml";
    mime[".xls"] = "application/vnd.ms-excel";
    mime[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime[".xml"] = "application/xml";
    mime[".xul"] = "application/vnd.mozilla.xul+xml";
    mime[".zip"] = "application/zip";
    mime[".3gp"] = "video/3gpp";
    mime[".3g2"] = "video/3gpp2";
    mime[".7z"] = "application/x-7z-compressed";
    mime[".mp4"] = "application/mp4";
    mime[".mp3"] = "audio/mpeg";
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
    errors = other.errors;
    mime = other.mime;
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
std::size_t Config::get_max_allowed_size() const
{
    std::size_t max_body;
    std::string tmp;
    std::string unity;
    std::stringstream ss;

    max_body = 0;
    unity = max_allowed_size[max_allowed_size.size()-1];
    tmp = max_allowed_size;
    if (!isdigit(unity[0]))
        tmp = (tmp.substr(0,tmp.size()-1));
    else
        unity = "" ;
    ss << tmp;
    ss >> max_body;
    if (unity == "k" || unity == "K")
        max_body *= 1000;
    else if (unity == "m" || unity == "M")
        max_body *= 1000000;
    else if (unity == "g" || unity == "G")
        max_body *= 1000000000;
    return max_body;
}
const std::vector<Location> &Config::get_locs() const
{
    return locs;
}
const ErrorPage &Config::get_errors() const
{
    return errors;
}
std::string Config::get_mime(const std::string &type)
{
    std::string s;

    s = "text/html";
    try
    {
        s = mime.at(type);
        return s;
    }
    catch(const std::exception &e)
    {
        (void)e;
        s = "text/plain";
    }
    return s;
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
    errors = err;
}

/*other get*/
Location    Config::get_location_match(std::string uri)
{
    Location *def;
    Location *re;
    std::vector<std::string> tmp1;
    int max;
    int t;

    def = NULL;
    re = NULL;
    max = -1;
    tmp1 = split(uri,"/");
    t = -1;
    for(std::vector<Location>::iterator i = locs.begin(); i != locs.end();i++)
    {
        if ((*i).get_uri() == "/")
        {
            def = &(*i);
            break; 
        }
    }
    
    for(std::vector<Location>::iterator i = locs.begin(); i != locs.end();i++)
    {
        std::vector<std::string> tmp;

        tmp = split((*i).get_uri(),"/");
        for(std::size_t j = 0; j < tmp.size();j++)
        {
            if (j < tmp1.size() && tmp1[j] == tmp[j])
                t ++;
        }
        if (max < t)
        {
            max = t;
            re = &(*i);
        }
        t = 0;
    }
    if (re == NULL)
        return (*def);
    return (*re);
}


std::string Config::get_real_path(std::string uri, Location loc)
{
    std::string real;
    std::size_t j;

    uri = decode_str(uri);
    real = loc.get_root();
    j = uri.find(loc.get_root());
    if (j == std::string::npos)
        return real;
    j += real.size();
    if (real[real.size()-1] == '/' && uri[0] == '/')
        real = real.substr(0,real.size()-1);
    real = "."+real;
    real += uri.substr(j,uri.size());
    return real;
}