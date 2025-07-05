#include "Cgi.hpp"

Cgi::~Cgi() {}

Cgi::Cgi(std::map<std::string, std::string> config, int length)
{
	argv = new char*[2];
	CgiName = new char;

	initEnvp(config, length);
	CgiName = const_cast<char*>("/usr/bin/php-cgi");
	argv[0] = CgiName;
	argv[1] = NULL;
}

std::string Cgi::parseUri(std::string BrutUri)
{
	std::string res = "";

	res = BrutUri.substr(2, BrutUri.find("?") - 2);
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

void	Cgi::initEnvp(std::map<std::string, std::string> config, int length)
{
	envp = new char*[8];
	std::stringstream c;
	std::string cl;
	std::string query = "";

	c << length;
	c >> cl;
	query = config["uri"];
	if (query.find("?") != std::string::npos)
		query = query.substr(query.find("?") + 1, query.length() - 1);
	std::string *tmp = new std::string[7]();
	tmp[0] = "GATEWAY_INTERFACE=CGI/1.1",
	tmp[1] = "REQUEST_METHOD=" + config["method"];
	if (parseUri(config["uri"]) == "")
		tmp[2] = "SCRIPT_FILENAME=error.html";
	else
		tmp[2] = "SCRIPT_FILENAME=" + parseUri(config["uri"]);
	tmp[3] = "QUERY_STRING=" + query;
    tmp[4] = "SERVER_PROTOCOL=HTTP/1.1";
	tmp[5] = "CONTENT_LENGTH=" + cl;
    tmp[6] = "REDIRECT_STATUS=200";
	for (size_t i = 0; i < 7; i++)
		envp[i] = const_cast<char*>(tmp[i].c_str());
	envp[7] = NULL;
}

// void	Cgi::sendPdf(std::string path, int socket)
// {
// 	std::cout << "PATH: |" << path << "|" << std::endl;
// 	std::ifstream img(path.c_str(), std::ios::binary);

// 	if (img.fail())
// 	{
// 		perror("IMG MISY BLEM");
// 		exit(0);
// 	}

// 	std::stringstream tmp;

// 	tmp << img.rdbuf();
// 	std::string rp;
// 	std::string h;
// 	size_t		lgth;

// 	rp = tmp.str();
// 	lgth = rp.size();
// 	h = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(lgth)  + "\r\nContent-Type: applications/pdf\r\n\r\n";
// 	send(socket, h.c_str(), h.size(), 0);
// 	send(socket, rp.c_str(), rp.size(), 0);
// }

void    Cgi::MyExec(int fdc)
{
	int fd[2];
	int pid;

	std::string a = envp[2];
	if (a.find(".png") != std::string::npos)
	{
		std::cout << RED << "A = " << a << R << std::endl;
		sendImg(a.substr(a.find("=") + 1, a.length() - 1), fdc);
		return;
	}
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
		int n;
		close(fd[1]);
		while ((n = read(fd[0], buff, 100)) > 0)
			p.append(buff, n);
		std::cout << "P:" << p << std::endl;
		close(fd[0]);
		if (send(fdc, p.c_str(), p.size(), 0) < 0)
		{
			std::cout << "EXECVE ERROR" << std::endl;
			exit(0);
		}
	}
}