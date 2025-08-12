/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/12 08:56:50 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

std::vector<std::string> split(std::string str, std::string sep)
{
        std::vector<std::string> nw;
        std::string tmp;
        std::size_t end;
        std::size_t start;

        tmp = "";
        start = 0;
        end = str.find(sep);
        while(end != std::string::npos)
        {
            tmp = str.substr(0,end);
            if (!tmp.empty())
                nw.push_back(tmp);
            start = end + sep.length();
            if (start >= str.length())
            {
                str = "";
                break;
            }
            str = str.substr(start, str.length());
            end = str.find(sep);
            tmp = "";
        }
        if (str != "")
            nw.push_back(str);
        return nw;
}

std::string get_html_page(int fd)
{
    std::string reponse;
    char buff[127];
    std::size_t len;
    
    std::memset(buff,0,126);
    len = read(fd,buff,126);
    while(len > 0)
    {
        reponse.append(buff,len);
        std::memset(buff,0,126);
        len = read(fd,buff,126);
    }
    return reponse;
}

int fd_is_ready(std::string path, Pollfd *polls, std::map<std::string, int> &fd_wait)
{
    int fd;

    fd = -1;
    try
    {
        fd = fd_wait.at(path);
        if ((polls->get_status(fd) & POLLIN))
            return fd;
    }
    catch(const std::exception &e)
    {
        fd = open(path.c_str(), O_RDWR);
        if (fd < 1)
            throw std::logic_error("error open");
        (void)e;
        polls->add_new_fd(fd);
        fd_wait[path] = fd;
    }
    return -1;
}

void  fd_closed(int fd,Pollfd *polls, std::map<std::string, int> &fd_wait, std::string path)
{
    polls->erase_fd(fd);
    fd_wait.erase(path);
}

bool is_directory(std::string path)
{
    struct stat file_s;

    std::memset(&file_s,0,sizeof(file_s));
    stat(path.c_str(), &file_s);
    if(S_ISDIR(file_s.st_mode))
        return true;
    return false;
}


std::string decode_str(std::string d)
{
    std::string tmp;

    tmp = "";

    for(std::size_t i = 0; i < d.size();i++)
    {
         if (d[i] == '%' && i+2 < d.size() && d[i+1] == '2' && d[i+2] == '0')
         {
            tmp += " ";
            i+=2;
         }
         else
            tmp += d[i];
    }
    return tmp;
}

void    free_addrinfo(std::vector<struct addrinfo *> &struct_addr)
{
    for(std::size_t i = 0; i < struct_addr.size();i++)
    {
        if (struct_addr[i])
        {
                freeaddrinfo(struct_addr[i]);
                struct_addr[i] = NULL;
        }
    }
}

std::string get_mime_type(std::string type)
{
    std::map<std::string, std::string> mime;

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