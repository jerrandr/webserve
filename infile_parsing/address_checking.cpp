/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address_checking.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:13:25 by randrina          #+#    #+#             */
/*   Updated: 2025/08/06 09:13:26 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

static int  check_one_add(std::string add)
{
    std::stringstream   content;
    int                 nbr;
    size_t              inc;
    char                pt;

    if (add == "localhost")
        return (1);
    std::vector<std::string> add_arr = split(add, ".");
    if (add_arr.size() != 4)
        return (0);
    inc = 0;
    while (inc < add.size())
    {
        if (!isdigit(add[inc]) && add[inc] != '.')
            return (0);
        inc ++;
    }
    content << add;
    while (!content.fail())
    {
        content >> nbr;
        content >> pt;
        if (nbr < 0 || nbr > 255 || pt != '.')
            return (0);
        inc ++;
    };
    return (1);
};

int     address_check(std::string   host, Config &cfg)
{
    std::string     one_add;
    std::string     last_host;
    size_t          pos;
    int             len;
    int             nbr;

    (void) cfg;
    pos = 0;
    nbr = 0;
    last_host = host;
    while (pos < host.size())
    {
        len = last_host.find(" ");
        one_add = host.substr(pos, len);
        if (!check_one_add(one_add))
        {
            std::cout << one_add << ": Ip address not valid" << std::endl;
            return (0);
        }
        nbr ++;
        if (len == -1)
            break;
        pos += len + 1;
        last_host = host.substr(pos, host.size() - pos);
    }
    cfg.set_host(host);
    return (nbr);
};