/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandling.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:22:22 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/15 09:43:15 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SignalHandling.hpp"


SignalHandling::SignalHandling()
{
    
}

SignalHandling::~SignalHandling()
{
    
}
SignalHandling::SignalHandling(const SignalHandling &other)
{
    (void)other;
}
SignalHandling &SignalHandling::operator=(const SignalHandling &other)
{
    (void)other;
    return (*this);
}




SignalHandling::ExceptSTop::ExceptSTop()
{
    m = "Server stop ...";
}
SignalHandling::ExceptSTop::~ExceptSTop() throw()
{
    
}

SignalHandling::ExceptSTop::ExceptSTop(const ExceptSTop &other)
{
    *this = other;
}

SignalHandling::ExceptSTop &SignalHandling::ExceptSTop::operator=(const ExceptSTop &other)
{
    m = other.m;
    return *this;
}

const char * SignalHandling::ExceptSTop::what() const throw()
{
    return m.c_str();
}
void SignalHandling::handle_signal(int sign)
{
   if (sign > 0)
        throw ExceptSTop() ;
}