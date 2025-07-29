/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/29 10:39:51 by msalohy          ###   ########.fr       */
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