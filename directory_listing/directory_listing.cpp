/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_listing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 09:11:57 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/10 13:49:34 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client.hpp"

static bool is_directory(struct dirent *direct, std::string name)
{
    struct stat file_s;
    std::string dir;

    std::memset(&file_s,0,sizeof(file_s));
    dir = direct->d_name;
    stat((name+dir).c_str(), &file_s);
    if(S_ISDIR(file_s.st_mode))
        return true;
    return false;
}


static std::string build_html_page(std::vector<std::string> files,std::string path, std::vector<std::string> name)
{
    std::string head;
    std::string rest;
    std::vector<std::string> tmp;
    std::string path_c;

    rest = "";
    head = "<!DOCTYPE html>\n   <html lang=\"en\">\n      <head>\n  <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Document</title>\n";
    head += "<style>a{color: black;text-decoration:none;}.container{display: flex;justify-content: space-between;align-content: space-between;margin:5px} .title{font-family: arial;margin : 0}";
    head += ".content{width:20%;} #head{margin-bottom: 20px;margin-top:20px;color: #023e3f;font-family: sans-serif;} #head .content{border-bottom:1px solid  #023e3f}";
    head += "body{margin:0}</style>";
    head += "</head><body><div class =\"content-head\"><center class=\"title\"><h1>INDEX OF "+path+"</h1></center>";
    head += "<div class=\"container\" id=\"head\">\n<div class =\"content\">Name</div> <div class =\"content\">Last modified</div><div class =\"content\">Size</div></div></div>";
    for(size_t i = 0; i < files.size();i++)
    {
        tmp = split(files[i],"\n");
        rest+="<div class=\"container\">\n";
        if (tmp.size()> 0)
        {
            path_c =path + name[i];
            // std::cout << name[i] << "]" << RED << std::endl;
            // if (name[i].find(path) == std::string::npos)
            //     path_c = path + path_c;
            rest += "<div class =\"content\"><a href=\""+path_c+"\">"+tmp[0]+"</a></div>";
            for(std::size_t j = 1; j < tmp.size(); j++)
            {
                rest += "<div class =\"content\">"+tmp[j]+"</div>";
            }
        }
        rest += "</div>";
    }
    head += rest +"\n</body\\>\n</html>";

    std::stringstream ss;

    ss << head.size();
    head  = "HTTP/1.1 200 OK\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;              
    
    return head;
}

static std::string build_html_page_error(int err,Config config, Pollfd *polls,std::map<std::string, int> &fd_wait)
{
    std::string head;
    int fd;
    
    head = "";
    fd = -1;
    if (err == 404)
    {
        if (access(config.get_errors().get_path_404().c_str(),F_OK | R_OK) < 0)
	    {
		    head = "<center><h1>404  Not Found </h1></center>";
	    }
        else
        {
            fd = fd_is_ready(config.get_errors().get_path_404(),polls,fd_wait);
            if (fd < 0)
                throw NotReady("404");
            head = get_html_page(fd);
            fd_closed(fd,polls,fd_wait,config.get_errors().get_path_404());
        }
    }
    else if(err == 403)
    {
        if (access(config.get_errors().get_path_403().c_str(),F_OK | R_OK) < 0)
	    {
		    head = "<center><h1>403  Permission Denied </h1></center>";
	    }
        else
        {
            fd = fd_is_ready(config.get_errors().get_path_403(),polls,fd_wait);
            if (fd < 0)
                throw NotReady("403");
            head = get_html_page(fd);
            fd_closed(fd,polls,fd_wait,config.get_errors().get_path_403());
        }
    }
    return head;
}

static bool access_denied(std::string &dir, std::string name, Config config, Pollfd *polls,std::map<std::string, int> &fd_wait)
{
    std::stringstream ss;

    
    if(access(name.c_str(),F_OK) < 0)
    {
        dir = build_html_page_error(404,config,polls,fd_wait);
        ss << dir.size();
        dir  = "HTTP/1.1 404 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ dir;              
    
        return true;
    }
    else if(access(name.c_str(),R_OK) < 0)
    {
        dir = build_html_page_error(403,config,polls,fd_wait);
        ss << dir.size();
        dir  = "HTTP/1.1 403 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ dir;
        return true;
    }
    return false;
}




static void do_directory(std::vector<std::string> &test, std::vector<std::string> &file_name, std::string dir, std::string name)
{
    struct stat file_s;
    std::string tmp;
    std::time_t tim;
    std::tm *tm_n;
    char buff[100];

    stat((name+dir).c_str(), &file_s);
    tim = file_s.st_mtime;
    tm_n = localtime(&tim);
    std::strftime(buff,sizeof(buff),"%d-%m-%Y %H:%M", tm_n);
    tmp = buff;
    test.push_back(dir+"/\n"+tmp+"\n-");
    file_name.push_back(dir+"/");
}

static  void    do_files(std::vector<std::string> &test, std::vector<std::string> &file_name, std::string dir, std::string name)
{
    std::stringstream ss;
    std::string tmp;
    float size;
    char buff[100];
    std::time_t tim;
    std::tm *tm_n;
    struct stat file_s;

    stat((name+dir).c_str(), &file_s);
    tim = file_s.st_mtime;
    tm_n = localtime(&tim);
    std::strftime(buff,sizeof(buff),"%d-%m-%Y %H:%M", tm_n);
    tmp = buff;
    size = file_s.st_size;
    if (size < 1000)
    {
        ss.clear();
        ss << size;
        test.push_back(dir+"\n" +tmp+"\n"+ss.str()+" o");
    }
    else if(size >= 1000 && size < (1000000))
    {
        size = size / 1000;
        ss.clear();
        ss << size;
        test.push_back(dir+"\n" +tmp+"\n"+ss.str()+" ko");
    }
    else if(size >= (1000000) && size <= (1000000000))
    {
        size = size / (1000000);
        ss.clear();
        ss << size;
        test.push_back(dir+"\n" +tmp+"\n"+ss.str()+" mo");
    }
    else
    {
        size = size / (1000000000);
        ss.clear();
        ss << size;
        test.push_back(dir+"\n" +tmp+"\n"+ss.str()+" go");
    }
    file_name.push_back(dir);
}
std::string    Client::directory_listing(std::string name, std::string uri)
{
    DIR *dirp;
    struct dirent *direct;
    std::vector<std::string> test;
    std::vector<std::string> file_name;
    std::string dir;
    std::stringstream ss;

    dir = "";
    dirp = NULL;
    std::cout << "path = " << name << std::endl;
    if(access_denied(dir,name,config,polls,fd_wait) == false)
    {
        dirp = opendir(name.c_str());
        if (dirp == NULL)
            return dir;
        direct = readdir(dirp);
        if (name[name.size()-1] != '/')
            name += '/';
        while(direct != NULL)
        {
            dir = direct->d_name;
            if(is_directory(direct, name))
                do_directory(test,file_name,dir,name);
            else
                do_files(test,file_name,dir,name);
            direct = readdir(dirp);
        }
        dir = "";
        Location locs = this->config.get_location_match(uri);
        dir = build_html_page(test,detranslate_uri(locs,uri),file_name);
    }
    if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
        send(socket, dir.c_str(), dir.size(), 0);
    if(dirp)
        closedir(dirp);
    return dir;
}