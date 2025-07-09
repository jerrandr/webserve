#include "Cgi.hpp"

Cgi::~Cgi() {}

Cgi::Cgi(std::map<std::string, std::string> config, int length, Pollfd *p, std::vector<Location> l)
{
	argv = new char*[2];
	CgiName = new char;

	pl = p;
	lc = l;
	lv = length;
	rq = config;
	CgiName = const_cast<char*>("/usr/bin/php-cgi");
	argv[0] = CgiName;
	argv[1] = NULL;
}

std::string Cgi::parseUri(std::string BrutUri)
{
	std::string res = "";

	if (BrutUri.length() >= 3)
		res = BrutUri.substr(1, BrutUri.find("?") - 2);
	return (res);
}

void	Cgi::sendImg(std::string path, int socket)
{
	std::cout << "PATH: |" << path << "|" << std::endl;
	std::ifstream img(path.c_str(), std::ios::binary);

	if (img.fail())
	{
		perror("IMG MISY BLEM");
		exit(0);
	}

	std::stringstream tmp;

	tmp << img.rdbuf();
	std::string rp;
	std::string h;
	size_t		lgth;

	rp = tmp.str();
	lgth = rp.size();
	h = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(lgth)  + "\r\nContent-Type: image/png\r\n\r\n";
	send(socket, h.c_str(), h.size(), 0);
	send(socket, rp.c_str(), rp.size(), 0);
}

void	Cgi::initEnvp(std::string rt)
{
	envp = new char*[8];
	std::stringstream c;
	std::string cl;
	std::string query = "";

	c << lv;
	c >> cl;
	query = rq["uri"];
	if (query.find("?") != std::string::npos)
		query = query.substr(query.find("?") + 1, query.length() - 1);
	std::string *tmp = new std::string[7]();
	tmp[0] = "GATEWAY_INTERFACE=CGI/1.1",
	tmp[1] = "REQUEST_METHOD=" + rq["method"];
	if (rt == "")
		tmp[2] = "SCRIPT_FILENAME=error.html";
	else
		tmp[2] = "SCRIPT_FILENAME=" + rt;
	tmp[3] = "QUERY_STRING=" + query;
    tmp[4] = "SERVER_PROTOCOL=HTTP/1.1";
	tmp[5] = "CONTENT_LENGTH=" + cl;
    tmp[6] = "REDIRECT_STATUS=200";
	for (size_t i = 0; i < 7; i++)
		envp[i] = const_cast<char*>(tmp[i].c_str());
	envp[7] = NULL;
}

void	Cgi::sendPdf(std::string path, int socket)
{
	std::cout << "PATH: |" << path << "|" << std::endl;
	std::ifstream img(path.c_str(), std::ios::binary);

	if (img.fail())
	{
		perror("IMG MISY BLEM");
		exit(0);
	}

	std::stringstream tmp;

	tmp << img.rdbuf();
	std::string rp;
	std::string h;
	size_t		lgth;

	rp = tmp.str();
	lgth = rp.size();
	h = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(lgth)  + "\r\nContent-Type: applications/pdf\r\n\r\n";
	send(socket, h.c_str(), h.size(), 0);
	send(socket, rp.c_str(), rp.size(), 0);
}

std::string Cgi::findLoc()
{
	for (std::vector<Location>::iterator i = lc.begin(); i < lc.end(); i++)
	{
		std::cout << "tO_FIND: " << (*i).get_uri() << " | URI: " << rq["uri"] << std::endl;  
		if ((*i).get_uri() == rq["uri"])
			return ((*i).get_root());
	}
	return ("");
}

void    Cgi::MyExec(int fdc)
{
	int fd[2];
	int pid;

	initEnvp(findLoc());
	std::string a = envp[2];
	std::cout << RED << "A = " << a << R << std::endl;
	// if (a.find(".png") != std::string::npos)
	// {
	// 	std::cout << RED << "A = " << a << R << std::endl;
	// 	sendImg(a.substr(a.find("=") + 1, a.length() - 1), fdc);
	// 	return;
	// }
	// else if (a.find(".pdf") != std::string::npos)
	// {
	// 	std::cout << RED << "A = " << a << R << std::endl;
	// 	sendPdf(a.substr(a.find("=") + 1, a.length() - 1), fdc);
	// 	return;
	// }
	if (pipe(fd) < 0)
		exit(0);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execve("/usr/bin/php-cgi", argv, envp);
		exit(0);
	}
	else
	{
		waitpid(pid, NULL, 0);
		std::string p = "HTTP/1.1 200 OK\r\n";	
		char buff[1024];
		int n = 1;
		memset(buff, 0, 1024);
		int s = 0;
		close(fd[1]);
		while ((n = read(fd[0], buff, 20)) > 0)
		{
			std::string tmp = buff;
			p.append(buff, n);
			s += n;
		}
		p.insert(p.find("\n") + 1, ("Content-Length: " + std::to_string(s - 59) + "\r\n") + ("Connection: keep-alive\r\n"));
		std::cout << "P: " << p << std::endl;
		close(fd[0]);
		if (pl->get_status(fdc) & POLLIN)
		{
			if (send(fdc, p.c_str(), p.size(), 0) < 0)
			{
				std::cout << "EXECVE ERROR" << std::endl;
				exit(0);
			}
		}
	}
}
