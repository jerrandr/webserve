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

static int      check_port_host(Config cfg)
{
    std::vector<std::string> host_array = split(cfg.get_host(), " ");
    std::vector<std::string> port_array = split(cfg.get_port(), " ");
    size_t              i = 0;
    size_t              j;
    std::string         tmp;
    std::string         tmp1;

    while (i < host_array.size() - 1)
    {
        j = i + 1;
        tmp = host_array[i] + port_array[i];
        while (j < host_array.size())
        {
            tmp1 = host_array[j] + port_array[j];
            if (tmp == tmp1)
            {
                std::cout << host_array[i] << "-" << port_array[i] << "==" << host_array[j] << 
                "-" << port_array[j] << ": Duplication of addresse and port" << std::endl;
                return (0);
            }
            j++;
        };
        i ++;
    }
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

static void default_port_adding(Config &cfg, int to_add)
{
    std::string new_port = cfg.get_port();
    for (int i =0; i < to_add; i ++)
        new_port += " 8080";
    cfg.set_port(new_port);
};

int         check_port_and_host(Config &cfg)
{
    int    listen_count;
    int    port_count;
    std::vector<std::string> listen_array = split(cfg.get_host(), " ");
    std::vector<std::string> port_array = split(cfg.get_port(), " ");
    std::vector<std::string>::iterator listen_it = listen_array.begin();
    std::vector<std::string>::iterator port_it = port_array.begin();
    listen_count = 0;
    port_count = 0;
    while (listen_it != listen_array.end())
    {
        listen_it ++;
        listen_count ++;
    };
    while (port_it != port_array.end())
    {
        port_it ++;
        port_count ++;
    };
    if (listen_count < port_count)
    {
        std::cout << "Number of host less than port" << std::endl;
        return (0);
    }
    if (listen_count > port_count)
        default_port_adding(cfg, listen_count - port_count);
    if (!check_port_host(cfg))
        return (0);
    return (1);
};