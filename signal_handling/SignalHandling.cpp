/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandling.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:22:22 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/29 08:43:12 by jerrandr         ###   ########.fr       */
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