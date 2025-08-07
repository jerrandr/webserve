/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_handling.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:29:29 by randrina          #+#    #+#             */
/*   Updated: 2025/08/07 10:29:33 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

static int  page_name_checking(std::string page_name)
{
    for (size_t i = 0; i << page_name.size(); i ++)
    {
        if (!isdigit(page_name[i]))
        std::cout << "Error on error_page name" << std::endl;
        return (0);  
    };
    if (std::atoi(page_name.c_str()) < 400 || std::atoi(page_name.c_str()) > 599)
    {
        std::cout << "Error on erro_page name" << std::endl;
        return (0);
    }
    return (1);
};

static int  page_path_checking(std::string path)
{
    std::cout << "paths_page-> " << path << std::endl;
    return (1);
};

int error_page_set(std::string path)
{
    std::string     page_name;
    std::string     page_path;
    int             last;

    last = path.find(" ");
    page_name = path.substr(0, last);
    page_path = path.substr(last + 1, path.size() - (last + 1));
    page_name_checking(page_name);
    page_path_checking(page_path);
    std::cout << "page_name-> " << page_name << "-page_path-> " << page_path << std::endl;
    return (1);
};