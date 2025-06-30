/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:34:25 by msalohy           #+#    #+#             */
/*   Updated: 2025/06/28 11:19:45 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
        socket = -1;
        requette = "";
        status_connexion = 0;
        status_requette = 0;
        size_body = 0;
        body = "";
        stat = 0;
}
Client::~Client()
{

}
Client::Client(const Client &other)
{
        *this = other;
}
Client &Client::operator=(const Client &other)
{
        socket = other.socket;
        requette = other.requette;
        status_connexion = other.status_connexion;
        status_requette = other.status_requette;
        size_body = other.size_body;
        body = other.body;
        stat = other.stat;
        return (*this);
}
        

Client::Client(int s,std::string req)
{
        socket = s;
        requette = req;
        reponse = "";
        status_connexion = 0;
        status_requette = 0;
        size_body = 0;
        body = "";
        stat = 0;
}
std::string Client::get_requette() const
{
        return requette;
}

int Client::get_socket_client() const
{
        return socket;
}


size_t Client::get_len_real_body()
{
        size_t size;
        std::string len;
        std::stringstream ss;

        size = 0;
        len = "";
        size =  requette.find("Content-Length:");
        if(size == std::string::npos)
                return 0;
        size = size + 16;
        for(size_t i = size; i < requette.length();i++ )
        {
                if(requette[i] == '\r' && i+1 < requette.length() && requette[i+1] == '\n')
                        break;
                len+= requette[i];
        }
        ss << len;
        ss >> size;
        return (size);
}


size_t Client::get_len_body(std::string buffer)
{
        size_t  start; 

        start = 0;
        start = buffer.find("\r\n\r\n");
        if (start == std::string::npos)
                return 0;
        if(start+4 < buffer.length())
                return 0;
        return (start+4);
}

void    Client::set_status_connexion( int status)
{
        status_connexion = status;
}

void    Client::send_message()
{
        std::string test;
        std::string html;
        std::ifstream fd("../socket/www/index.html");
        std::stringstream ss;
        int size;

        test = "";
        if(fd==0)
                std::perror("Error");
        size = 0;
        while(std::getline(fd,html))
        {
                size += html.length();
                test += html;
        }
        ss << size;
        test = "HTTP/1.1 200 OK\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ test;
        size = send(socket,test.c_str(),strlen(test.c_str()),0);
        if(size == -1)
                std::perror("error 1");
        status_requette = 0;
}

int     Client::get_status() const
{
        return stat;
}

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

void    Client::set_head(int size,char buffer[1024])
{
        if(requette != "")
                return ;
       if(size == 0)
                return ;
        else
        {
                for(int i = 0; i < size ; i++)
                {
                        requette += buffer[i];
                }
        }
}
static std::string get_body(std::string buffer, int &size)
{
        std::string body;
        std::string head;
        size_t start;

        body = "";
        head = "";
        start = 0;
        for(size_t i = 0; i < buffer.size(); i++)
        {
                if(buffer[i] == ' ')
                {
                        if((head == "GET") || (head == "POST") || (head == "POST")
                                || (head == "POST")|| (head == "HEAD") || (head == "OPTIONS")
                                 ||(head == "TRACE") || (head == "CONNECT") || (head == "PATCH"))
                        {
                                for(size_t j = i ; j < buffer.size(); j++)
                                {
                                        if(j +3 < buffer.size() && buffer[j] == '\r'
                                                && buffer[j+1] == '\n' && buffer[j+2] == '\r' && buffer[j+3] == '\n')
                                        {
                                                start = j+4;
                                                break;
                                        }
                                        head += buffer[j];
                                        i = j;
                                }
                        }
                        if(start != 0)
                                break;
                }
                head += buffer[i];
        }
        size = start;
        while(start < buffer.size())
        {
                body += buffer[start];
                start ++;
        }
        return body;
}

void    Client::body_split()
{
        std::string new_body;
        std::vector<std::string> spl;

        new_body = "";
        spl = split(body,"\r\n");
        if(spl.size() > 5)
        {
                for(size_t i = 3 ; i < spl.size(); i++)
                {
                        if(i < spl.size()-1)
                                new_body += spl[i];
                }
        }
        else
        {
                 for(size_t i = 3 ; i < spl.size(); i++)
                {
                        new_body += spl[i];
                }
        }
        if(new_body != "")
                body = new_body;
}
void    Client::receve_message()
{
        char buffer[1024];
        int status;
        size_t real_body;
        int size;
        
        real_body = 0;
        size = 0;
        bzero(buffer,sizeof(buffer));
        status = 0;
        status =recv(socket,buffer,1024-1,0) ;
        if(status < 0)
                return ;
        else if(status == 0)
        {
                std::cout << "Client deconnected" << std::endl;
                stat = -1;
                return ;
        }
        body += get_body(buffer, size);
        set_head(size,buffer);
        real_body = get_len_real_body();
        size_body += status - size;
        status_requette = 1+ (real_body - size_body);
        if(status_requette == 0 || status_requette == 1)
        {
                status_requette = 1;
                requette += body;
                std::cout << "------------------message--------------------" << std::endl;
                std::cout  << body <<std::endl;
                std::cout << "----------------------------------------------" << std::endl;        
                parse_requette();
                requette = "";
                size_body = 0;
                body = "";
        }
        
}

void    Client::parse_requette()
{
        std::map<std::string, std::string> config;
        std::string temp1;
        std::string temp2;

        temp1 = "";
        temp2 = "";
        for(size_t i = 0; i < requette.size();i++)
        {
                if (requette[i] == ' ' && ((temp1 == "GET") || (temp1 == "POST") || (temp1 == "POST")
                                || (temp1 == "POST") || (temp1 == "HEAD") || (temp1 == "OPTIONS")
                                 || (temp1 == "TRACE") || (temp1 == "CONNECT") || (temp1 == "PATCH")))
                {
                        config["method"] = temp1;
                        temp1 = "";
                }
                else if (requette[i] == ' ' && (temp1.find("/") != std::string::npos && temp1 != "HTTP/1.1"))
                {
                        config["uri"] = temp1;
                        temp1 = "";
                }
                else if(temp1.find("HTTP/1.1") != std::string::npos)
                {
                        temp1 = "";
                }
                else if(requette[i]==':' && temp1[temp1.size()-1] != ':')
                {
                        temp1 += requette[i];
                        i ++;
                        if(i < requette.size() && requette[i] == ' ')
                                i++;
                        for(size_t j = i; j < requette.size(); j++)
                        {
                                if(j+1 < requette.size() && requette[j] == '\r' && requette[j+1] == '\n')
                                        break;
                                temp2 += requette[j];
                        }
                        if(temp1 != " " && temp2 != " ")
                        {
                                config[temp1] = temp2;
                                temp2 = "";
                        }
                        temp1 = "";
                }
                temp1 += requette[i];                 
        }
        body_split();
        if(config["method"] == "POST")
        {
                std::ofstream fd("test.out");

                fd << body;
        }
}
void    Client::set_requette(std::string &n)
{
        requette = n;
}

void    Client::verify_connex(int status)
{
        if (status == 1)
        {
                receve_message();
        }
        else if (status == 2 && status_requette == 1)
        {
                send_message();
                
        }
}