/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:46:21 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/18 13:54:01 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
{
    path_400 = "error/400.html";
    path_403 = "error/403.html";
    path_404 = "error/404.html";
    path_405 = "error/405.html";
    path_406 = "error/406.html";
    path_408 = "error/408.html";
    path_410 = "error/410.html";
    path_411 = "error/411.html";
    path_413 = "error/413.html";
    path_414 = "error/414.html";
    path_415 = "error/415.html";
    path_417 = "error/417.html";
    path_500 = "error/500.html";
    path_501 = "error/501.html";
    path_502 = "error/502.html";
    path_503 = "error/503.html";
    path_504 = "error/504.html";
    path_505 = "error/505.html";
}

ErrorPage::~ErrorPage()
{
    
}
ErrorPage::ErrorPage(const ErrorPage &other)
{
    *this = other;
}
ErrorPage &ErrorPage::operator=(const ErrorPage &other)
{
    path_400 = other.path_400;
    path_403 = other.path_403;
    path_404 = other.path_404;
    path_405 = other.path_405;
    path_406 = other.path_406;
    path_408 = other.path_408;
    path_410 = other.path_410;
    path_411 = other.path_411;
    path_413 = other.path_413;
    path_414 = other.path_414;
    path_415 = other.path_415;
    path_417 = other.path_417;
    path_500 = other.path_500;
    path_501 = other.path_501;
    path_502 = other.path_502;
    path_503 = other.path_503;
    path_504 = other.path_504;
    path_505 = other.path_505;
    return *this;
}
/*getters*/
const std::string &ErrorPage::get_path_400() const
{
    return path_400;
}
const std::string &ErrorPage::get_path_403() const
{
    return path_403;
}
const std::string &ErrorPage::get_path_404() const
{
    return path_404;
}
const std::string &ErrorPage::get_path_405() const
{
    return path_405;
}
const std::string &ErrorPage::get_path_406() const
{
    return path_406;
}
const std::string &ErrorPage::get_path_408() const
{
    return path_408;
}
const std::string &ErrorPage::get_path_410() const
{
    return path_410;
}
const std::string &ErrorPage::get_path_411() const
{
    return path_411;
}
const std::string &ErrorPage::get_path_413() const
{
    return path_413;
}
const std::string &ErrorPage::get_path_414() const
{
    return path_414;
}
const std::string &ErrorPage::get_path_415() const
{
    return path_415;
}
const std::string &ErrorPage::get_path_417() const
{
    return path_417;
}
const std::string &ErrorPage::get_path_500() const
{
    return path_500;
}
const std::string &ErrorPage::get_path_501() const
{
    return path_501;
}
const std::string &ErrorPage::get_path_502() const
{
    return path_502;
}
const std::string &ErrorPage::get_path_503() const
{
    return path_503;
}
const std::string &ErrorPage::get_path_504() const
{
    return path_504;
}
const std::string &ErrorPage::get_path_505() const
{
    return path_505;
}

/*setters*/
void    ErrorPage::set_path_400(std::string &path)
{
    path_400 = path;
}
void    ErrorPage::set_path_403(std::string &path)
{
    path_403 = path;
}
void    ErrorPage::set_path_404(std::string &path)
{
    path_404 = path;
}
void    ErrorPage::set_path_405(std::string &path)
{
    path_405 = path;
}
void    ErrorPage::set_path_406(std::string &path)
{
    path_406 = path;
}
void    ErrorPage::set_path_408(std::string &path)
{
    path_408 = path;
}
void    ErrorPage::set_path_410(std::string &path)
{
    path_410 = path;
}
void    ErrorPage::set_path_411(std::string &path)
{
    path_411 = path;
}
void    ErrorPage::set_path_413(std::string &path)
{
    path_413 = path;
}
void    ErrorPage::set_path_414(std::string &path)
{
    path_414 = path;
}
void    ErrorPage::set_path_415(std::string &path)
{
    path_415 = path;
}
void    ErrorPage::set_path_417(std::string &path)
{
    path_417 = path;
}
void    ErrorPage::set_path_500(std::string &path)
{
    path_500 = path;
}
void    ErrorPage::set_path_501(std::string &path)
{
    path_501 = path;
}
void    ErrorPage::set_path_502(std::string &path)
{
    path_502 = path;
}
void    ErrorPage::set_path_503(std::string &path)
{
    path_503 = path;
}
void    ErrorPage::set_path_504(std::string &path)
{
    path_504 = path;
}
void    ErrorPage::set_path_505(std::string &path)
{
    path_505 = path;
}