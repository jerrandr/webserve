#include "Cgi.hpp"

Cgi::~Cgi() {}

Cgi::Cgi(std::map<std::string, std::string> config)
{
	argv = new char*[2];
	CgiName = new char;

	initEnvp(config);
	// std::cout << "envp:\n";
	// std::cout << envp[0] << std::endl;
	// std::cout << envp[1] << std::endl;
	// std::cout << envp[2] << std::endl;
	CgiName = const_cast<char*>("/usr/bin/php-cgi");
	argv[0] = CgiName;
	argv[1] = NULL;

	std::cout << "CgiName: " << CgiName << std::endl;
	std::cout << "argv[0]: " << argv[0] <<std::endl;
	std::cout << "argv[1]: " << argv[1] << std::endl;
}

void	Cgi::initEnvp(std::map<std::string, std::string> config)
{
	envp = new char*[4];
	
	std::string *tmp = new std::string[3]();
	tmp[0] = "SCRIPT_FILENAME=/home/jerrandr/Videos/webserve/test.php";
	tmp[1] = "REQUEST_METHOD=" + config["method"];
	if (config["method"] == "GET")
		tmp[2] = "QUERY_STRING=test";
	else
		tmp[2] = "CONTENT_LENGTH=";
	for (size_t i = 0; i < 3; i++)
		envp[i] = const_cast<char*>(tmp[i].c_str());
	envp[3] = NULL;
}

void    Cgi::MyExec(int fdc)
{
	int fd[2];
	int pid;

	if (pipe(fd) < 0)
		exit(0);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execve("/usr/bin/php-cgi", argv, envp);
		std::perror("KO ");
		exit(0);
	}
	else
	{
		waitpid(pid, NULL, 0);
		std::cout << "envp:\n";
		std::cout << envp[0] << std::endl;
		std::cout << envp[1] << std::endl;
		std::cout << envp[2] << std::endl;
		std::cout << "++++++++++++++" << std::endl;
		std::string p = "HTTP/1.1 200 OK\r\n";
		char buff[1024];
		int n;
		close(fd[1]);
		while ((n = read(fd[0], buff, 100)) > 0)
			p.append(buff, n);
		close(fd[0]);
		if (send(fdc, p.c_str(), p.size(), 0) < 0)
		{
			std::cout << "EXECVE ERROR" << std::endl;
			exit(0);
		}
	}
}