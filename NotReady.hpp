/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NotReady.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:47:44 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/13 12:50:48 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTREADY_HPP
#define NOTREADY_HPP
#include "utils.h"

class NotReady : public std::exception
{
    private:
        std::string mess;
    public:
        NotReady(std::string e);
        NotReady();
        virtual ~NotReady() throw();
        NotReady(const NotReady &e);
        NotReady &operator=(const NotReady &e);
        
        virtual const char *what() const throw();
};
#endif