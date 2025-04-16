#include "../include/HTTPServer.hpp"

HTTPServer::HTTPServer(int port, std::unique_ptr<CacheStrategy> cache): 
port(port), cache(std::move(cache)), pool() {
    utils = std::make_unique<ServerUtils>(this);
};

void HTTPServer::start(){
    setupSocket();
    acceptConnections();
}

void HTTPServer::setupSocket(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // tells kernel to open a network socket, then returns a unique file descriptor, SOCK_STREAM:TCP based network stream, 0:default
    sockaddr_in address = {AF_INET, htons(port), INADDR_ANY}; // AF_INET:IPv4, htons:Converts the port from host byte order to network byte order(big-endian), INADDR_ANY:0.0.0.0
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 10); // 10 is the backlog — max number of connections waiting in the queue before they’re accepted, If the queue is full, new clients get ECONNREFUSED
}

void HTTPServer::acceptConnections(){
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr); // Accept a new connection on a socket that is listening for connections
        ClientHandlerTask task(client_fd, this);
        pool.submit(task);
    }
}

void HTTPServer::handleClient(int client_fd){ // the main logic
    char buffer[8192] = {0};
    read(client_fd, buffer, sizeof(buffer));
    std::string request(buffer);

    auto extractedUrlPtr = utils->extractURLFromRequest(request);
    if(extractedUrlPtr){
        auto method = extractedUrlPtr->first;
        auto normalizedUrlPtr = extractedUrlPtr->second;
        if (method == "GET"){
            utils->handleGET(client_fd, *normalizedUrlPtr);
        }else{
            // create a cacheResponse object
            // use sendResponse method to send the error
        }
    }else{
        // create a cacheResponse object
        // use sendResponse method to send the error
    }
    close(client_fd);
}

