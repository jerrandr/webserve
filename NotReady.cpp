/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NotReady.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:49:39 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/13 12:50:22 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NotReady.hpp"

NotReady::NotReady(std::string e)
{

    mess = e;
}
NotReady::NotReady()
{
    mess = "stop";
}
NotReady::~NotReady() throw()
{
    
}
NotReady::NotReady(const NotReady &e)
{
    *this = e;
}
NotReady &NotReady::operator=(const NotReady &e)
{
    mess = e.mess;
    return *this;
}
const char *NotReady::what() const throw()
{
    return mess.c_str();
}