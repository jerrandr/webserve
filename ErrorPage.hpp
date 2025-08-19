/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:13:16 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/22 07:47:10 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP
#include <iostream>
class ErrorPage
{
    private:
        std::string path_400;
        std::string path_403;
        std::string path_404;
        std::string path_405;
        std::string path_406;
        std::string path_408;
        std::string path_410;
        std::string path_411;
        std::string path_413;
        std::string path_414;
        std::string path_415;
        std::string path_417;
        std::string path_500;
        std::string path_501;
        std::string path_502;
        std::string path_503;
        std::string path_504;
        std::string path_505;

    public:
        ErrorPage();
        ~ErrorPage();
        ErrorPage(const ErrorPage &other);
        ErrorPage &operator=(const ErrorPage &other);

        /*getters*/
        const std::string &get_path_400() const;
        const std::string &get_path_403() const;
        const std::string &get_path_404() const;
        const std::string &get_path_405() const;
        const std::string &get_path_406() const;
        const std::string &get_path_408() const;
        const std::string &get_path_410() const;
        const std::string &get_path_411() const;
        const std::string &get_path_413() const;
        const std::string &get_path_414() const;
        const std::string &get_path_415() const;
        const std::string &get_path_417() const;
        const std::string &get_path_500() const;
        const std::string &get_path_501() const;
        const std::string &get_path_502() const;
        const std::string &get_path_503() const;
        const std::string &get_path_504() const;
        const std::string &get_path_505() const;

        /*setters*/
        void    set_path_400(std::string &path);
        void    set_path_403(std::string &path);
        void    set_path_404(std::string &path);
        void    set_path_405(std::string &path);
        void    set_path_406(std::string &path);
        void    set_path_408(std::string &path);
        void    set_path_410(std::string &path);
        void    set_path_411(std::string &path);
        void    set_path_413(std::string &path);
        void    set_path_414(std::string &path);
        void    set_path_415(std::string &path);
        void    set_path_417(std::string &path);
        void    set_path_500(std::string &path);
        void    set_path_501(std::string &path);
        void    set_path_502(std::string &path);
        void    set_path_503(std::string &path);
        void    set_path_504(std::string &path);
        void    set_path_505(std::string &path);
};

#endif