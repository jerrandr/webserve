/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/02 13:13:41 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "directory_listing.h"

std::vector<std::string> split(std::string str, std::string sep)
{
        size_t status;
        std::string tmp;
        std::vector <std::string> new_spl;

        status = 0;
        tmp = "";
        for(size_t i = 0; i < str.size();i++)
        {
                status = 0;
                if(str[i] != sep[0])
                {
                        for(size_t j = i; j < str.size(); j++)
                        {
                                if(str[j] == sep[0])
                                {
                                    break;
                                }
                                tmp += str[j];
                                status += 1;
                        }
                        i+= status;
                        status = 0;
                        if(str[i] == sep[0])
                        {
                            while(true)
                            {
                                status = 0;
                                for(size_t j = 0; j < sep.size(); j++)
                                {
                                    if(str[i+j] != sep[j])
                                        break;
                                    status +=1;
                                }
                                i += status;
                                if(str[i]!= sep[0])
                                    break;
                        }
                        if(status >= sep.size())
                        {
                                i -= 1;
                                new_spl.push_back(tmp);
                                tmp = "";
                        }
                        }
                }
        }
        if(tmp != "")
            new_spl.push_back(tmp);
        return new_spl;
}