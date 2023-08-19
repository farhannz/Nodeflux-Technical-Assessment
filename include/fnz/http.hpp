#ifndef INCLUDE_FNZ_HTTP_H
#define INCLUDE_FNZ_HTTP_H

#include <libasyik/service.hpp>
#include <libasyik/http.hpp>
#include <iostream>
#include <iomanip>
#include <spdlog/spdlog.h>
namespace fnz::http{

    class Server{
        private:
            std::string addr;
            int port;
        public:
            asyik::service_ptr as;
            asyik::http_server_ptr<asyik::http_stream_type> server;
            Server(const std::string& addr = "0.0.0.0", const int port = 8080);
            void init();
            void run();
            template <typename B, typename M, typename R>
            void request(R&&route, M&&m, B&&cb);
            template <typename B, typename M, typename R>
            void request_regex(R&&route, M&&m, B&&cb);
            
    };


    Server::Server(const std::string& addr, const int port) : addr(addr), port(port){}
    void Server::init(){
        this->as = asyik::make_service();
        this->server = asyik::make_http_server(this->as,this->addr,this->port);
    }

    void Server::run(){
        spdlog::info("{} STARTING SERVER {}", std::string(8,'='), std::string(8,'='));
        spdlog::info("LISTENING AT {} {}", this->addr, this->port);
        this->as->run();
    }
    template <typename B, typename M, typename R>
    void Server::request(R&&route,  M&&m, B&& cb){
        this->server->on_http_request(route, m, cb);
    }
    template <typename B, typename M, typename R>
    void Server::request_regex(R&&route,  M&&m, B&& cb){
        this->server->on_http_request_regex(route, m, cb);
    }

}

#endif