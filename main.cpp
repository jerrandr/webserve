#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

int main()
{
    struct addrinfo tt;
    struct addrinfo *tt1;
    int     fdS = 0;
    int     fdC = 0;

    memset(&tt, 0, sizeof(struct addrinfo));
    tt.ai_family = AF_INET;
    tt.ai_socktype = SOCK_STREAM;
    getaddrinfo("localhost", "8080", &tt, &tt1);
    fdS = socket(tt1->ai_family, tt1->ai_socktype, tt1->ai_protocol);
    if (fdS == -1)
    {
        std::cout << "error1";
        return (-1);
    }
    if (bind(fdS, tt1->ai_addr, tt1->ai_addrlen) == -1)
    {
        std::cout << "error2";
        return (-1);
    }
    listen(fdS, 10);
    fdC = accept(fdS, NULL, NULL);
    if (fdC == -1)
    {
        std::cout << "error3";
        return (-1);
    }
    else
        std::cout << "METY" << std::endl;
    char tp[1024];
    bzero(tp, 1024);
    recv(fdC, tp, 20, 0);
    send(fdC, "VAOVAO\n", 7, 0);
    std::cout << "TP: " << tp << std::endl;
    close(fdS);
    close(fdC);
    return (0);
}