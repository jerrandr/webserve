/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_listing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 09:11:57 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/02 14:51:53 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "directory_listing.h"
static std::string build_html_page(std::vector<std::string> files,std::string path, std::vector<std::string> name)
{
    std::string head;
    std::string rest;
    std::vector<std::string> tmp;

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
            rest += "<div class =\"content\"><a href=\""+path+name[i]+"\">"+tmp[0]+"</a></div>";
            for(std::size_t j = 1; j < tmp.size(); j++)
            {
                rest += "<div class =\"content\">"+tmp[j]+"</div>";
            }
        }
        rest += "</div>";
    }
    head += rest +"\n</body\\>\n</html>";

    return head;
}

static std::string build_html_page_error(int err)
{
    std::string head;
    std::string buff;
    
    head = "";
    buff = "";
    if (err == 404)
    {
        std::ifstream fd("../error/404.html");
        if (!fd)
            return head;
        while(getline(fd,buff))
            head += buff;
    }
    else if(err == 403)
    {
        std::ifstream fd("../error/403.html");
        if (!fd)
            return head;
        while(getline(fd,buff))
            head += buff;
    }
    return head;
}

static bool access_denied(std::string &dir, std::string name)
{
    if(access(name.c_str(),F_OK) < 0)
    {
        dir = build_html_page_error(404);
        return true;
    }
    else if(access(name.c_str(),R_OK) < 0)
    {
        dir = build_html_page_error(403);
        return true;
    }
    return false;
}

static bool is_directory(struct dirent *direct, std::string name)
{
    struct stat file_s;
    std::string dir;
            
    dir = direct->d_name;
    stat((name+dir).c_str(), &file_s);
    if(S_ISDIR(file_s.st_mode))
        return true;
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
    file_name.push_back(dir);
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
std::string    directory_listing(std::string &name)
{
    DIR *dirp;
    struct dirent *direct;
    std::vector<std::string> test;
    std::vector<std::string> file_name;
    std::string dir;

    dir = "";
    dirp = NULL;
    if(access_denied(dir,name) == false)
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
            {
                do_directory(test,file_name,dir,name);
            }
            else
            {
                do_files(test,file_name,dir,name);
            }
            direct = readdir(dirp);
        }
        dir = "";
        dir = build_html_page(test,name,file_name);
    }
    {
        std::ofstream fd("index.html");

        fd << dir;
    }
    if(dirp)
        closedir(dirp);
    return dir;
}