/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:30:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:32:52 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Socket.hpp"
#include "response/Cgi.hpp"
#include "response/Response.hpp"

class Client
{
    private:    
        Config config;
        Pollfd *polls;
        int socket;
        std::string request;
        std::string reponse;
        std::string body;
        size_t real_body;
        int status_request;
        int status_connexion;
        size_t size_body;
        int stat;
        std::map<std::string, int> fd_wait;
        ssize_t request_time;
        ssize_t client_timeout;
        Client();
    public:
        //+++++++++++++++++++++    
        int     pid;
        int     fd_in;
        int     fd_out;
        bool    fl;
        time_t  bg;
        //+++++++++++++++++++++
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);
        

        Client(int s, Pollfd *poll, Config &conf);
        int get_socket_client() const;
        std::string get_request() const;
        void    set_request(std::string n);
        int     get_status() const;
        int     get_status_request();
        void    set_socket(int s);

        void    set_status_connexion( int status);

        void    receve_message();

        void    verify_connex(int status);

        size_t get_len_real_body();
        size_t get_len_body(std::string buffer);
        void    set_head(int size,std::string buffer);

        void max_body_size_trait();
        std::size_t size_fd_wait();


        int is_dir_listing(std::string uri);
        std::string    directory_listing(std::string name, std::string uri);
        void    exec_dir_listing(std::string uri);
        

        bool    is_bad_request();
        void    exec_bad_request();


        bool    other_traitment(std::map<std::string, std::string> config);


        ssize_t get_timeout();
        void    set_timeout(ssize_t t);
        void    exec_request_timeout();
        void    read_timeout();


        ssize_t get_timeout_client();

        void exec_http_not_supported();

        void    exec_error_server();
        void    error_serv();

		Pollfd		*getPoll() const;
		Config		getConfig() const;
        std::map<std::string, int>  &getFdWait();
        std::string	getBody() const;
        void    parse_request();
        void    body_unchunked();

        void    set_status_client(int s);



        bool    is_not_implemented(std::map<std::string, std::string> cf);
        void    exec_not_implemented();

        void    exec_500();

        bool is_chunked(std::string buffer);

        bool    is_post();
        bool    is_len_required();
        void    exec_len_required();
    };
#endif
