/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandling.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:02:44 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/09 08:56:01 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLING_HPP
#define SIGNALHANDLING_HPP
#include "../utils.h"
class SignalHandling
{
    private:
        SignalHandling();
        ~SignalHandling();
        SignalHandling(const SignalHandling &other);
        SignalHandling &operator=(const SignalHandling &other);
    public:

        static void handle_signal(int sign);

        class ExceptSTop: public std::exception
        {
            private:
                std::string m;
            public:
                ExceptSTop();
                ~ExceptSTop();
                ExceptSTop(const ExceptSTop &other);
                ExceptSTop &operator=(const ExceptSTop &other);
                virtual const char * what() const throw();
                
        };
};

#endif