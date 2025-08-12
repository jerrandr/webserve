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

int     method_check(std::string value)
{
    std::string erase_space;
    std::string check_value;
    std::string last_value;
    int         index;

    index = 0;
    while (isspace(value[index]))
        index ++;
    erase_space = value.substr(index, value.size() - index);
    std::cout << "value_method-> [" << erase_space << "]" << std::endl;
    return (1);
};

int     root_check(std::string value)
{
    std::cout << "valu-> " << value << std::endl;
    return (1);
};

int     autoindex_check(std::string value, Location &lcs)
{
    if (value == "YES")
        lcs.set_enabled(1);
    else if (value == "NO")
        lcs.set_enabled(0);
    else
    {
        std::cout << "autoindex value not accepted !!!" << std::endl;
        return (0);
    }
    return (1);
}
int     index_check(std::string value)
{
    std::cout << "valu-> " << value << std::endl;
    return (1);
};

int     upload_check(std::string value)
{
    std::cout << "valu-> " << value << std::endl;
    return (1);
};

int     cgi_check(std::string value)
{
    std::cout << "valu-> " << value << std::endl;
    return (1);
}

int     redirect_check(std::string value)
{
    std::cout << "valu-> " << value << std::endl;
    return (1);
};

int     cgi_path_check(std::string value)
{
    std::cout << "valu-> " << value << std::endl;
    return (1);
}
int     cgi_script_check(std::string value)
{
    std::cout << "valut-> " << value << std::endl;
    return (1);
};

