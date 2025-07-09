/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_listing.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 09:09:53 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/02 14:51:37 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORY_LISTING_H
#define DIRECTORY_LISTING_H
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <ctime>
std::string    directory_listing(std::string &name);
std::vector<std::string> split(std::string str, std::string sep);


#endif