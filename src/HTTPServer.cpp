#include "../include/HTTPServer.hpp"

HTTPServer::HTTPServer(int port, std::unique_ptr<CacheStrategy> cache): 
port(port), cache(std::move(cache)), pool() {

};

void HTTPServer::start(){
    setupSocket();
    acceptConnections();
}

void HTTPServer::setupSocket(){
    
}

void HTTPServer::acceptConnections(){
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        ClientHandlerTask task(client_fd, this);
        pool.submit(task);
    }
}

void HTTPServer::handleClient(int client_fd){

}

