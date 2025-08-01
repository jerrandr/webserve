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

static void     get_all_line(std::string input)
{
    int         len;
    size_t      pos;
    std::string line;
    std::string new_input;

    pos = 0;
    new_input = input;
    while (pos < input.size())
    {
        len = new_input.find("\n");
        line = input.substr(pos,len);
        pos += len+1;
        new_input = input.substr(pos,input.size() - pos);
        std::cout << new_input << std::endl;
    };
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