/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:30:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/09 12:32:11 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Socket.hpp"
#include "response/Cgi.hpp"
#include "response/Requette.hpp"

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
        std::map<std::string, int> fd_wait;
        ssize_t request_time;
        ssize_t client_timeout;
        Client();
    public:
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);
        

        Client(int s, Pollfd *poll, Config &conf);
        int get_socket_client() const;
        std::string get_requette() const;
        void    set_requette(std::string n);
        int     get_status() const;
        int     get_status_requette();

        void    set_status_connexion( int status);

        void    receve_message();
        void    send_message();

        void    verify_connex(int status);

        size_t get_len_real_body();
        size_t get_len_body(std::string buffer);
        void    set_head(int size,std::string buffer);

        /*max body size*/
        void max_body_size_trait();
        std::size_t size_fd_wait();


        /*dir list*/
        int is_dir_listing(std::string uri);
        std::string    directory_listing(std::string name);
        void    exec_dir_listing(std::string uri);
        
        /*bad request*/
        bool    is_bad_request();
        void    exec_bad_request();

        /*modif page */
        bool    other_traitment(std::map<std::string, std::string> config);
        // std::vector<std::string>    body_split();


        /*pour 408*/
        ssize_t get_timeout();
        void    set_timeout(ssize_t t);
        void    exec_request_timeout();
        void    read_timeout();

        /*pour timeout client*/
        ssize_t get_timeout_client();
        /*pour 505*/
        void exec_http_not_supported();
        /*pour 500*/
        void    exec_error_server();
        void    error_serv();
		// JERRY MODIF
		Pollfd		*getPoll() const;
		Config		getConfig() const;
        std::map<std::string, int>  &getFdWait();
        std::string	getBody() const;
        void    parse_requette();
        void    body_unchunked();

        void    set_status_client(int s);


        /*pour 501*/
        bool    is_not_implemented(std::map<std::string, std::string> cf);
        void    exec_not_implemented();
};
#endif
