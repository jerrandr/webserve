/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location_check.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:28:37 by randrina          #+#    #+#             */
/*   Updated: 2025/08/12 10:28:40 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int     method_check(std::string value, std::vector<std::string> &found_key)
{
    std::vector<std::string>  splited = split(value, " ");
    std::vector<std::string>::iterator it = splited.begin();
    int         count = 0;

    found_key.push_back("method");
    if (value.empty())
        return (0);
    while (it != splited.end())
    {
        if (*it != "GET" && *it != "POST" && *it != "DELETE")
            return (0);
        it ++;
        count ++;
    }
    if (count > 3)
        return (0);
    return (1);
};

int     root_check(std::string value, std::vector<std::string> &found_key)
{
    std::vector<std::string> spl_value;

    found_key.push_back("root");
    if (value.empty())
        return (0);
    spl_value = split(value, " ");
    if (spl_value.size() != 1)
        return (0);
    return (1);
};

int     autoindex_check(std::string value, Location &lcs, std::vector<std::string> &found_key)
{
    found_key.push_back("autoindex");
    if (value == "YES" || value == "yes")
    {
        lcs.set_enabled(1);
        return (1);
    }
    else if (value == "NO" || value == "no")
    {
        lcs.set_enabled(0);
        return (1);
    }
    return (0);
};

int     index_check(std::string value, std::vector<std::string> &found_key)
{
    std::vector<std::string> spl_value;

    found_key.push_back("index");
    if (value.empty())
        return (0);
    spl_value = split(value, " ");
    if (spl_value.size() != 1)
        return (0);
    return (1);
};

int     upload_check(std::string value, std::vector<std::string> &found_key)
{
    std::vector<std::string> spl_value;

    found_key.push_back("upload");
    if (value.empty())
        return (0);
    spl_value = split(value, " ");
    if (spl_value.size() != 1)
        return (0);
    return (1);
};

int     cgi_check(std::string value, std::vector<std::string> &found_key)
{
    found_key.push_back("cgi");
    if (value.empty())
        return (0);
    return (1);
}

int     redirect_check(std::string value, std::vector<std::string> &found_key)
{
    std::vector<std::string> spl_value;

    found_key.push_back("redirect");
    if (value.empty())
        return (0);
    spl_value = split(value, " ");
    if (spl_value.size() != 2)
        return (0);
    return (1);
};

int     cgi_path_check(std::string value, std::vector<std::string> &found_key)
{
    std::vector<std::string> spl_value;

    found_key.push_back("cgi_path");
    if (value.empty())
        return (0);
    spl_value = split(value, " ");
    if (spl_value.size() != 1)
        return (0);
    return (1);
}
