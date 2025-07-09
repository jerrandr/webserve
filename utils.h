/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:35 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/09 13:51:31 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "Location.hpp"
#include "ErrorPage.hpp"
#include "Config.hpp"
#include "Pollfd.hpp"
#include "signal_handling/SignalHandling.hpp"
std::vector<std::string> split(std::string str, std::string sep);


#endif