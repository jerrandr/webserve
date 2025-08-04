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

static int      content_error_handling(std::string line)
{
    std::string all_keys[13] = {"server", "host", "listen", "error_page",
         "client_max_body_size", "local", "method", "root", "autoindex",
          "index", "route", "upload", "cgi"};
    /*int t = all_keys->find(line);
    std::cout << "content erro_handling" << all_keys[t] << std::endl;
    return (0);*/
    (void) line;
    return (1);
};

static void     line_parsing(std::string line)
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
    std::cout << key_w << std::endl;
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
        line_parsing(line);
        if (len == -1)
            break ;
        pos += len + 1;
        new_input = input.substr(pos,input.size() - pos);
    };
    content_error_handling("anana");
};

void    config_parsing(int fd)
{
    std::string string;
    size_t      length;
    char        buffer[100];

    length = 100;
    string = "";
    std::memset(buffer, 0, 100);
    while (read(fd, buffer, length - 1) > 0)
    {
        string +=  buffer;
        std::memset(buffer, 0, 100);
    };
    get_all_line(string);   
};