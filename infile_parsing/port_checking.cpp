/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   port_checking.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:32:53 by randrina          #+#    #+#             */
/*   Updated: 2025/08/06 15:32:54 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

static int      check_one_port(std::string  port)
{
    size_t     i = 0;

    while (i < port.size())
    {
        if (!isdigit(port[i]))
        {
            std::cout << "invalid port-> " << port << std::endl;
            return (0);
        }
        i ++;
    };
    return (1);
};

int     port_check(std::string port, Config &cfg)
{
    std::string one_port;
    std::string last_port;
    size_t      pos;
    int         len;
    int         nbr;

    (void) cfg;
    nbr = 0;
    pos = 0;
    last_port = port;
    while (pos < port.size())
    {
        len = last_port.find(" ");
        one_port = port.substr(pos, len);
        check_one_port(one_port);
        nbr ++;
        if (len == -1)
            break ;
        pos += len + 1;
        last_port = port.substr(pos, port.size() - pos);
    };
    cfg.set_port(port);
    return (nbr);
};