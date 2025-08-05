/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 09:11:58 by randrina          #+#    #+#             */
/*   Updated: 2025/08/01 09:12:00 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static int      keys_error_handling(std::string line)
{
    std::vector<std::string> all_keys = {"server", "host", "listen", "error_page",
         "client_max_body_size", "local", "method", "root", "autoindex",
          "index", "route", "upload", "cgi"};
    auto it = find(all_keys.begin(), all_keys.end(), line);
    if (it != all_keys.end())
        return (1);
    return (0);
};

static int     line_parsing(std::string line)
{
    int             last;
    size_t          i;
    std::string     key_w;
    std::string     new_line;
    i = 0;
    while (isspace(line[i]))
        i ++;
    new_line = line.substr(i, line.size() - i);
    last = new_line.find(" ");
    key_w = new_line.substr(0, last);
    if (!keys_error_handling(key_w))
        return (0);
    return (1);    
};

static void     get_all_line(std::string input)
{
    int                                 len;
    size_t                              pos;
    std::string                         line;
    std::string                         new_input;
    std::vector<std::string>            key_vect;

    pos = 0;
    new_input = input;
    while (pos < input.size())
    {
        len = new_input.find("\n");
        line = input.substr(pos,len);
        if (!line_parsing(line))
        {
            std::cout << "Error content" << std::endl;
            break;
        }
        if (len == -1)
            break ;
        pos += len + 1;
        new_input = input.substr(pos,input.size() - pos);
    };
};

void    config_parsing(int fd)
{
    std::string         string;
    size_t              length;
    char                buffer[100];
    std::vector<OscF>   all_server;
    std::vector<OscF>   oneserver;
    length = 100;
    string = "";
    std::memset(buffer, 0, 100);
    while (read(fd, buffer, length - 1) > 0)
    {
        string +=  buffer;
        std::memset(buffer, 0, 100);
    };
    get_all_line(string);
    all_server.push_back(oneserver);
};