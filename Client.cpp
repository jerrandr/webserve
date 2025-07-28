/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:34:25 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/28 07:38:39 by msalohy          ###   ########.fr       */
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
        real_body = 0;
}

Client::~Client() {}

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
        real_body = other.real_body;
        config = other.config;
        polls = other.polls;
        return (*this);
}
        

Client::Client(int s, Pollfd *poll, Config &conf)
{
        socket = s;
        requette = "";
        reponse = "";
        status_connexion = 0;
        status_requette = 0;
        size_body = 0;
        body = "";
        stat = 0;
        real_body = 0;
        config = conf;
        polls = poll;
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
        if (reponse == "")
        {
                std::string test;
                std::string html;
                std::ifstream fd("index.html");
                std::stringstream ss;
                int size;

                test = "";
                if(fd.fail())
                        std::perror("Error");
                size = 0;
                while(std::getline(fd,html))
                {
                        size += html.length();
                        test += html;
                }
                ss << size;
                // test = "HTTP/1.1 200 OK\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ test;
                size = send(socket,test.c_str(),strlen(test.c_str()),0);
                if(size == -1)
                        std::perror("error 1");
        }else
        {
                std::string test;
                int size;
                std::stringstream ss;

                size = 0;
                size += size_body;
                std::cout << "size body " << size_body << " " <<reponse.size()<<std::endl;
                ss << size_body;
                // test = "HTTP/1.1 200 OK\r\nContent-Length: "+ss.str()+"\r\nContent-Type: image/png\r\n\r\n";
                size += test.size();
                test.append(reponse,size_body);
                size += size_body;
                size = send(socket,&test[0],size,0);
                if(size == -1)
                        std::perror("error 1");
        }
        status_requette = 0;
}

int     Client::get_status() const
{
        return stat;
}

void    Client::set_head(int size,std::string buffer)
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
        std::vector <char> bo;
        size_t start;

        body = "";
        head = "";
        start = 0;
        for(size_t i = 0; i < buffer.size(); i++)
        {
                if(buffer[i] == ' ')
                {
                        if((head == "GET") || (head == "HEAD") || (head == "OPTIONS")
                                || (head == "TRACE")|| (head == "PUT") || (head == "DELETE")
                                 ||(head == "POST") || (head == "PATCH") || (head == "CONNECT"))
                        {
                                for(size_t j = i ; j < buffer.size(); j++)
                                {
                                        if(j +3 < buffer.size() && buffer[j] == '\r'
                                                && buffer[j+1] == '\n' && buffer[j+2] == '\r' && buffer[j+3] == '\n')
                                        {
                                                start = j+4;
                                                break;
                                        }
                                        head+= buffer[j];
                                        i = j;
                                }
                        }
                        if(start != 0)
                                break;
                }
                head+=buffer[i];
        }
        size = start;
        while(start < buffer.size())
        {
                bo.push_back(buffer[start]);
                start ++;
        }
        for(size_t i = 0; i < bo.size() ; i++)
                body += bo[i];
        return body;
}

// std::vector<std::string>     Client::body_split()
// {
//         std::string new_body;
//         std::vector<std::string> spl;
//         std::vector <std::string> r;

//         new_body = "";
//         std::ofstream fd("test.out");

//         fd << requette +body;
//         spl = split(body,"\r\n");
//         if(spl.size() >= 5)
//         {
//                 for(size_t i = 3 ; i < spl.size(); i++)
//                 {
//                         if(i < spl.size()-1)
//                                 new_body += spl[i];
//                 }
//         }
//         else
//         {
//                  for(size_t i = 3 ; i < spl.size(); i++)
//                 {
//                         new_body += spl[i];
//                 }
//         }
//         if(new_body != "")
//                 body = new_body;
//         for(size_t i = 1; i < 3; i++)
//         {
//                 if(i < spl.size())
//                         r.push_back(spl[i]);
//         }
//         return r;
// }

static  bool is_chunked(std::string buffer)
{
        std::size_t size;
        std::string tmp;

        tmp = "";
        size = buffer.find("Transfer-Encoding: ");
        if (size == std::string::npos)
                return false;
        for(std::size_t i = size; i < buffer.size(); i++)
        {
                if (i+1 < buffer.size() && buffer[i] == '\r'&& buffer[i+1] == '\n')
                        break;
                tmp += buffer[i];
        }
        if (tmp == "Transfer-Encoding: chunked")
                return true;
        return false;
}

int body_chunked(int len1 , std::string body)
{
        for(int i = 0; i < len1;i++)
        {
                if (i+4 < len1 && body[i] == '0'
                        && body[i+1] == '\r' && body[i+2] == '\n'
                        && body[i+3] == '\r' && body[i+4] == '\n')
                {
                        if ((i+5) >= len1)
                                return (0);
                }
        }
        return(1);
}

void    Client::body_unchunked()
{
        long current_size;
        std::string tmp;
        std::stringstream ss;
        std::string new_body;
        std::size_t temp;

        current_size = -1;
        tmp = "";
        temp = 0;
        new_body = "";
        real_body = 0;
        for(std::size_t i = 0; i < body.size();i++)
        {
                temp =body.find("\r\n") ;
                if (temp == std::string::npos)
                {
                        return;
                }
                tmp = body.substr(i,temp);
                ss << std::hex << tmp;
                ss >> current_size;
                real_body += current_size;
                i += temp+2;
                for(long j = 0 ; j < (current_size);j++)
                        new_body+= body[j+i];
                ss.clear();
                i += current_size+1;
                current_size = 0;
                tmp = "";
                temp = 0;
        }
        body = new_body;
}
void    Client::receve_message()
{
        // static int u = 0;
        char buffer[1024];
        int status;
        int size;
        
        size = 0;
        std::memset(buffer,0,sizeof(buffer));
        status = 0;
        status =recv(socket,buffer,1024-1,0) ;
        // u += status;
        // std::cout << buffer << std::endl;
        if(status < 0)
                return ;
        else if(status == 0)
        {
                // std::cout << "Client deconnected" << std::endl;
                stat = -1;
                return ;
        }
        std::string tmp;

        tmp.append(buffer,status);
        // std::cout << u << std::endl;
        // if (u == 2494804)
                // std::cout << buffer << std::endl;
        body += get_body(tmp, size);
        size_body += status - size;
        // std::cout << "{" << body << "}" << std::endl;
        set_head(size,tmp);
        // std::cout <<"{"<<buffer <<"}"<<std::endl;
        if (stat >= 1 || (is_chunked(tmp)))
        {
                stat = 1;
                // std::cout << "chunked" <<std::endl;
                stat =  body_chunked(status, tmp);
                if (stat == 0)
                {
                        std::cout << real_body << " " << size_body << std::endl;
                        // std::cout << "dude" << std::endl;
                        // std::cout << "{" << body << "}" << std::endl;
                        // std::cout << requette<< std::endl;
                        // std::cout << "end" << std::endl;
                        body_unchunked();
                        size_body = real_body;
                        stat = 0;
                        status_requette = 1;
                }
        }
        else if (stat != 1 )
        {
                if(real_body == 0)
                        real_body = get_len_real_body();
                if(real_body == size_body)
                {
                        status_requette = 1;
                }
                stat = 0;
        }
        if(status_requette == 1 && real_body == size_body)
        {
                status_requette = 1;
                status = 2;
                size_body = 0;
                // requette += body;
                // std::cout << "real_" << real_body << " " << size_body << std::endl;
       
                // std::cout << "------------------message--------------------" << std::endl;
                // std::cout  << body <<std::endl;
                // std::cout << "----------------------------------------------" << std::endl;        
                // std::cout << "start parse()" << std::endl;
                parse_requette();
                if (fd_wait.size() == 0)
                {
                        requette = "";
                        body = "";
                }
        }
        
}

void    Client::exec_dir_listing(std::string uri)
{
        Location loc;
        std::string path;


        loc = config.get_location_match(uri);
        path = config.get_real_path(uri,loc);
        directory_listing(path);
}
int Client::is_dir_listing(std::string uri)
{
        Location loc;
        std::string path;


        loc = config.get_location_match(uri);
        path = config.get_real_path(uri,loc);
        if (is_directory(path))
        {
                if (loc.get_directory_listing() && loc.get_index() == "")
                        return (1);
        }
        return (0);
}
void    Client::parse_requette()
{
        std::map<std::string, std::string> config;
        std::string temp1;
        std::string temp2;

        temp1 = "";
        temp2 = "";
        // std::cout <<"{"<<requette<<"}"<< std::endl;
        for(size_t i = 0; i < requette.size();i++)
        {
                if (requette[i] == ' ' && ((temp1 == "GET") || (temp1 == "HEAD") || (temp1 == "OPTIONS")
                                || (temp1 == "TRACE") || (temp1 == "PUT") || (temp1 == "DELETE")
                                 || (temp1 == "POST") || (temp1 == "PATCH") || (temp1 == "CONNECT")))
                {
                        config["method"] = temp1;
                        temp1 = "";
                        i += 1;
                }
                else if (requette[i] == ' ' && (temp1.find("/") != std::string::npos && temp1 != "HTTP/1.1"))
                {
                        config["uri"] = temp1;
                        temp1 = "";
                        i+=1;
                }
                else if(temp1.find("HTTP/1.1") != std::string::npos)
                {
                        config["http_version"] = temp1;
                        temp1 = "";
                        i += 2;
                }
                else if(requette[i]==':' && i+1 < requette.size() && requette[i+1] == ' ')
                {
                        i += 2;
                        for(size_t j = i; j < requette.size(); j++)
                        {
                                if(j+1 < requette.size() && requette[j] == '\r' && requette[j+1] == '\n')
                                {
                                        i += 2;
                                        break;
                                }
                                temp2 += requette[j];
                        }
                        i += temp2.size();
                        if(temp1 != " " && temp2 != " ")
                        {
                                // std::cout <<"{"<<temp1 <<"}{"<< temp2 <<"}"<<std::endl;
                                config[temp1] = temp2;
                                temp2 = "";
                        }
                        temp1 = "";
                        if (i  >= requette.size())
                                return ;
                }
                temp1 += requette[i];                 
        }
        // std::cout <<"{Methode}" << "{" << config["method"] <<"}"<<std::endl;
        // std::cout <<"{uri}" << "{" << config["uri"] <<"}"<<std::endl;
        // std::cout <<"{http_version}" << "{" << config["http_version"] <<"}"<<std::endl;
        // if(config["method"] == "POST")
        // {
        //         std::ofstream fd("test.out");
        //         std::cout << body << std::endl;
        //         fd << body;
        // }
        // std::cout << "link = " << config["uri"] << std::endl;
        if (this->config.get_max_allowed_size() < this->get_len_real_body())
        {
                max_body_size_trait();
                // requette = "";
                return ;
        }
        else if(is_dir_listing(config["uri"]))
        {
                try
                {
                        exec_dir_listing(config["uri"]);
                        requette = "";
                }
                catch(std::exception &e)
                {
                        (void)e;
                }
                return ;
        }
        // config["uri"] = this->config.get_real_path(config["uri"],this->config.get_location_match(config["uri"]));
        
        Requette a(config, *this);

        a.rp(socket);
}
void Client::max_body_size_trait()
{
        int fd;
        std::string path;
        std::string t;
        std::stringstream ss;

        path = this->config.get_errors().get_path_413();
        t = "";
        fd = -1;
        if (access(path.c_str(), F_OK| R_OK) < 0)
        {
                path = this->config.get_errors().get_path_500();
                if (access(path.c_str(), F_OK| R_OK) < 0)
                {
                        t = "500 INTERNAL SERVER ERROR";
                        ss << t.size();
                        t  = "HTTP/1.1 500 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ t;
                        if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
                                send(socket, t.c_str(), t.size(), 0);
                        return ;
                }
                fd = fd_is_ready(path,polls,fd_wait);
                if (fd != -1)
                {
                        t = get_html_page(fd);
                        ss << t.size();
                        t  = "HTTP/1.1 500 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ t;
                        if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
                                send(socket, t.c_str(), t.size(), 0);
                        fd_closed(fd,polls,fd_wait,path);
                                
                }
                return;
        }
        fd = fd_is_ready(path,polls,fd_wait);
        if (fd != -1)
        {
                t = get_html_page(fd);
                ss << t.size();
                t  = "HTTP/1.1 413 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ t;
                if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
                        send(socket, t.c_str(), t.size(), 0);
                fd_closed(fd,polls,fd_wait,path);                
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
        else if (status == 2 && status_requette == 1 && reponse != "" && fd_wait.size() == 0)
        {
                send_message();
                size_body = 0;
                real_body = 0;
                
        }
}
std::size_t Client::size_fd_wait()
{
        return fd_wait.size();
}
// JERRY MODIF
Pollfd *Client::getPoll() const
{
	return (polls);
}

Config Client::getConfig() const
{
	return (config);
}

std::string	Client::getBody() const
{
	return (body);
}