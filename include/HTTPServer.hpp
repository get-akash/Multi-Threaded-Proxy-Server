#pragma once

#include <string>
#include <netinet/in.h>
#include "./CacheStrategy.hpp"
#include "./ThreadPool.hpp"
#include "./ServerUtils.hpp"

class HTTPServer{
protected:
    std::unique_ptr<CacheStrategy> cache;

private:
    int server_fd;
    int port;
    unsigned int cacheSize;
    bool useLFU;
    ThreadPool pool;

    void setupSocket();
    void acceptConnections();
    void handleClient();

public:
    HTTPServer(int port, unsigned int cacheSize, bool useLFU = false);
    void start();
};