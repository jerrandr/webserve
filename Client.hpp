/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:30:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/07 10:48:14 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Socket.hpp"
#include "Cgi.hpp"

class Client
{
    private:
        Config config;
        Pollfd *polls;
        int socket;
        std::string requette;
        std::string reponse;
        std::string body;
        size_t real_body;
        int status_requette;
        int status_connexion;
        size_t size_body;
        int stat;
        Client();
    public:
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);
        

        Client(int s, Pollfd *poll, Config &conf);
        int get_socket_client() const;
        std::string get_requette() const;
        void    set_requette(std::string &n);
        int     get_status() const;

        void    set_status_connexion( int status);

        void    receve_message();
        void    send_message();

        void    verify_connex(int status);

        size_t get_len_real_body();
        size_t get_len_body(std::string buffer);
        void    set_head(int size,std::string buffer);
        // std::vector<std::string>    body_split();

        void    parse_requette();
};
#endif