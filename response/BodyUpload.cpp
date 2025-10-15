/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/13 12:54:02 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"
#include "BodyUpload.hpp"
#define RED "\033[31m"
#define R "\033[0m"

BodyUpload::~BodyUpload()
{
	delete utils;
}

BodyUpload::BodyUpload(std::string rt) : Rt(rt)
{
	utils = new ExecUtils();
}

std::string	BodyUpload::ParseHeader(std::string header)
{
	std::string res;
	int			start;
	int			end;

	start = 0;
	end = 0;
	res = "";
	if (header.find("filename") != std::string::npos)
	{
		res = header.substr(header.find("filename"), header.find("\r\n"));
		start = res.find("\"");
		res = res.substr(start + 1, res.length());
		end = res.find("\"");
		res = res.substr(0, end);
	}
	return (res);
}

const char * BodyUpload::Error409::what() const throw()
{
	return ("");
}

std::string	BodyUpload::getSep(std::string bd)
{
	std::string	res;

	res = bd.substr(0, bd.find("\r\n"));
	return (res);
}

int  BodyUpload::fd_create(std::string path, Pollfd *polls, std::map<std::string, int> &fd_wait)
{
    int			fd;
    fd = -1;

    try
    {
        fd = fd_wait.at(path);
        if ((polls->get_status(fd) & POLLIN))
            return fd;
    }
    catch(const std::out_of_range &e)
    {
		if (is_directory(path))
			throw (BodyUpload::Error409());
		fd = open(path.c_str(), O_CREAT | O_WRONLY, 0666);
        if (fd < 0)
			throw std::bad_alloc();
        (void)e;
        polls->add_new_fd(fd);
        fd_wait[path] = fd;
    }
    return fd;
}

std::string	BodyUpload::rp_201()
{
	std::string	nbr;
	std::string	rp;
	std::string	body;

	body = "<!DOCTYPE html><html lang=\"fr\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Upload Réussi</title><style>body {background: linear-gradient(135deg, #1a002b, #4b0082);color: white;font-family: \"Poppins\", sans-serif;display: flex;flex-direction: column;justify-content: center;align-items: center;height: 100vh;margin: 0;}.card {background: rgba(255, 255, 255, 0.05);border: 1px solid rgba(255, 255, 255, 0.15);border-radius: 20px;padding: 50px 70px;text-align: center;box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5);animation: fadeIn 0.8s ease;backdrop-filter: blur(8px);}h1 {font-size: 2.3em;margin-bottom: 10px;color: #d1aaff;}p {font-size: 1.1em;opacity: 0.9;color: #e5ccff;}.checkmark {font-size: 70px;color: #bb86fc;margin-bottom: 15px;animation: pop 0.6s ease;}@keyframes fadeIn {from {opacity: 0;transform: translateY(20px);}to {opacity: 1;transform: translateY(0);}}@keyframes pop {0% {transform: scale(0);}60% {transform: scale(1.2);}100% {transform: scale(1);}}</style></head><body><div class=\"card\"><div class=\"checkmark\">✔</div><h1>Upload réussi !</h1><p>Votre fichier a été téléchargé avec succès.</p></div></body></html>";
	nbr = utils->ToString(body.length());
	rp = "HTTP/1.1 201 Created\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + body;
	return (rp);
}

void BodyUpload::ParseBody(Client &cl)
{
	std::string		header;
	std::string		ct;
	std::string		filename;
	std::string		sep;
	std::string		Bdy;
	
	Bdy = cl.getBody();
	sep = getSep(Bdy);
	
	while (Bdy.find("\r\n\r\n") != std::string::npos)
	{
		header = Bdy.substr(0, Bdy.find("\r\n\r\n"));
		Bdy = Bdy.substr(header.length() + 4, Bdy.length());
		ct = Bdy.substr(0, (Bdy.find(sep) - 1));
		header = ParseHeader(header);
		filename = Rt + header;
		if (Bdy.length() < ct.size() + 4)
			break;
		
		Bdy = Bdy.substr(ct.size() + 4, Bdy.length());
		if (header != "")
		{
			filPath.push_back(header);
			fd_create(filename, cl.getPoll(), cl.getFdWait());
			vl.insert(std::pair<std::string, std::string>(filename, ct));
		}
	}	
}

void	BodyUpload::UploadHandler(Client &cl)
{
	int	fd;

	fd = 0;
	ParseBody(cl);
	if (vl.size() > 0)
	{
		for (std::map<std::string, std::string>::iterator i = vl.begin(); i != vl.end(); i++)
		{
			
			if ((fd = fd_create((*i).first, cl.getPoll(), cl.getFdWait())) != -1)
			{
				write(fd, (*i).second.data(), (*i).second.size());
				fd_closed(fd, cl.getPoll(), cl.getFdWait(), (*i).first);
			}
		}	
	}
}