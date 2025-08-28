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

static int     path_set(int page_ind, ErrorPage &err_page, std::string err_path)
{
    switch (page_ind)
    {
    case 400:
        err_page.set_path_400(err_path);
        break;
    case 403:
        err_page.set_path_403(err_path);
        break;
    case 404:
        err_page.set_path_404(err_path);
        break;
    case 405:
        err_page.set_path_405(err_path);
        break;
    case 406:
        err_page.set_path_406(err_path);
        break;
    case 408:
        err_page.set_path_408(err_path);
        break;
    case 410:
        err_page.set_path_410(err_path);
        break;
    case 411:
        err_page.set_path_411(err_path);
        break;
    case 413:
        err_page.set_path_413(err_path);
        break;
    case 414:
        err_page.set_path_414(err_path);
        break;
    case 415:
        err_page.set_path_415(err_path);
        break;
    case 417:
        err_page.set_path_417(err_path);
        break;
    case 500:
        err_page.set_path_500(err_path);
        break;
    case 501:
        err_page.set_path_501(err_path);
        break;
    case 502:
        err_page.set_path_502(err_path);
        break;
    case 503:
        err_page.set_path_503(err_path);
        break;
    case 504:
        err_page.set_path_504(err_path);
        break;
    case 505:
        err_page.set_path_505(err_path);
        break;
    default:
        return (0);
    }
    return (1);
};

int  error_page_occurences(std::vector<std::string> &error_vect)
{
    if (error_vect.size() == 0)
        return (1);
    for (size_t i = 0; i < error_vect.size() - 1; i ++)
    {
        for (size_t j = i +1; j < error_vect.size(); j ++)
        {
            if (error_vect[i] == error_vect[j])
                return (0);
        }
    }
    return (1);
};


int error_page_set(std::string path, ErrorPage &err_page, std::vector<std::string> &err_vect)
{
    std::string     page_name;
    std::string     page_path;
    int             err_set;
    int             last;

    last = path.find(" ");
    page_name = path.substr(0, last);
    err_vect.push_back(page_name);
    page_path = path.substr(last + 1, path.size() - (last + 1));
    if (access(page_path.c_str(), F_OK || R_OK) == -1)
        return (0);
    err_set = std::atoi(page_name.c_str());
    if (!path_set(err_set, err_page, page_path))
        return (0);
    return (1);
};