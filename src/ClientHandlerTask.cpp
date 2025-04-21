#include "../include/ClientHandlerTask.hpp"
#include "../include/HTTPServer.hpp"

ClientHandlerTask::ClientHandlerTask(int _client_fd, HTTPServer* _server):
        server(_server), client_fd(_client_fd) {};

void ClientHandlerTask::operator()(){
    server->handleClient(client_fd);
};