/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location_parsing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:39:00 by randrina          #+#    #+#             */
/*   Updated: 2025/08/08 14:39:01 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int     get_principal_uri(std::string value, Config &cfg)
{
    Location    locs;
    std::string uri_value;
    int         last;

    last = value.find(" ");
    uri_value = value.substr(0, last);
    std::cout << "get_principal_uri-> [" << value << "]" << std::endl;
    locs.set_uri(uri_value);
    cfg.set_locs(locs);
    return (1);
};