/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:46:21 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 11:16:56 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
{
    path_404 = "error/404.html";
    path_500 = "error/500.html";
    path_403 = "error/403.html";
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
    path_404 = other.path_404;
    path_500 = other.path_500;
    path_403 = other.path_403;
    return *this;
}

/*getters*/
const std::string &ErrorPage::get_path_404() const
{
    return path_404;
}
const std::string &ErrorPage::get_path_403() const
{
    return path_404;
}
const std::string &ErrorPage::get_path_500() const
{
    return path_500;
}

/*setters*/
void    ErrorPage::get_path_404(std::string &path)
{
    path_404 = path;
}
void    ErrorPage::get_path_403(std::string &path)
{
    path_403 = path;
}
void    ErrorPage::get_path_500(std::string &path)
{
    path_500 = path;
}