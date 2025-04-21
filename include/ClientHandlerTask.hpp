#pragma once

class HTTPServer;

class ClientHandlerTask{
private:
    int client_fd;
    HTTPServer* server;

public:
    ClientHandlerTask(int _client_fd, HTTPServer* _server);

    void operator()();
};