#pragma once

#include <string>
#include <netinet/in.h>
#include "./CacheStrategy.hpp"
#include "./ThreadPool.hpp"
#include "./ServerUtils.hpp"
#include "./ClientHandlerTask.hpp"

class HTTPServer{
protected:
    std::unique_ptr<CacheStrategy> cache;

private:
    int server_fd;
    int port;
    ThreadPool pool;

    void setupSocket();
    void acceptConnections();

public:
    HTTPServer(int port, std::unique_ptr<CacheStrategy> cache);
    void start();
    void handleClient(int client_fd);
};