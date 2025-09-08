/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeoutHandling.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 08:44:34 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/29 13:24:31 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMEOUTHANDLING_CPP
#define TIMEOUTHANDLING_CPP
#include "Requette.hpp"

class timeoutHandling:std::exception
{
	public:
		timeoutHandling();
		// ~timeoutHandling();
		virtual const char * what() const throw();
};

void    handling(int signal);
#endif