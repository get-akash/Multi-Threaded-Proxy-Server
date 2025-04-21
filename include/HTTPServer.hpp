#pragma once

#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include "./CacheStrategy.hpp"
#include "./ThreadPool.hpp"
#include "./ClientHandlerTask.hpp"
#include "./ServerUtils.hpp"

class HTTPServer{
friend class ServerUtils;
protected:
    std::unique_ptr<CacheStrategy> cache;

private:
    int server_fd;
    int port;
    ThreadPool pool;
    std::unique_ptr<ServerUtils> utils;

    void setupSocket();
    void acceptConnections();

public:
    HTTPServer(int port, std::unique_ptr<CacheStrategy> cache);
    void start();
    void handleClient(int client_fd); // the main logic
};