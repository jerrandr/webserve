/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:13:16 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/01 11:16:47 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP
#include <iostream>
class ErrorPage
{
    private:
        std::string path_404;
        std::string path_500;
        std::string path_403;
    public:
        ErrorPage();
        ~ErrorPage();
        ErrorPage(const ErrorPage &other);
        ErrorPage &operator=(const ErrorPage &other);

        /*getters*/
        const std::string &get_path_404() const;
        const std::string &get_path_403() const;
        const std::string &get_path_500() const;

        /*setters*/
        void    get_path_404(std::string &path);
        void    get_path_403(std::string &path);
        void    get_path_500(std::string &path);
};

#endif