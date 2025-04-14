#pragma once
#include "./HTTPServer.hpp"

class ClientHandlerTask{
private:
    int client_fd;
    HTTPServer* server;

public:
    ClientHandlerTask(int _client_fd, HTTPServer* _server):
        server(_server), client_fd(_client_fd) {};

    void operator()(){
        server->handleClient(client_fd);
    };
};