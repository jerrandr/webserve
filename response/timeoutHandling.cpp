/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeoutHandling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 08:30:31 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/29 13:45:11 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Requette.hpp"
#include "timeoutHandling.hpp"

timeoutHandling::timeoutHandling() {}

// timeoutHandling::~timeoutHandling() {}

const char * timeoutHandling::what() const throw()
{
    return ("");
}

void    handling(int signal)
{
    std::cerr << "HEREEEEEEEEEEEEEE\n";
    if (signal == SIGALRM)
        throw timeoutHandling();
}