#include "../include/HTTPServer.hpp"
#include "../include/ClientHandlerTask.hpp"
#include "../include/ServerUtils.hpp"
#include <functional>

HTTPServer::HTTPServer(int port, std::unique_ptr<CacheStrategy> cache): 
port(port), cache(std::move(cache)), pool() {
    utils = std::make_unique<ServerUtils>(this);
};

void HTTPServer::start(){
    setupSocket();
    acceptConnections();
}

void HTTPServer::setupSocket(){
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // tells kernel to open a network socket, then returns a unique file descriptor, SOCK_STREAM:TCP based network stream, 0:default
    if (server_fd == -1) {
        std::cerr << "Error creating socket." << std::endl;
        exit(EXIT_FAILURE);  // Exit if socket creation fails
    }
    
    sockaddr_in address;
    address.sin_family = AF_INET; // AF_INET:IPv4,
    address.sin_port = htons(port); // htons:Converts the port from host byte order to network byte order(big-endian)
    address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY:0.0.0.0

    // bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        exit(EXIT_FAILURE);  // Exit if bind fails
    }


    // listen(server_fd, 10); // 10 is the backlog — max number of connections waiting in the queue before they’re accepted, If the queue is full, new clients get ECONNREFUSED
    if (listen(server_fd, 10) == -1) {
        std::cerr << "Error listening on socket." << std::endl;
        exit(EXIT_FAILURE);  // Exit if listen fails
    }
}

void HTTPServer::acceptConnections(){
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr); // Accept a new connection on a socket that is listening for connections
        std::cout << "[accept] Received client_fd = " << client_fd << std::endl;

        auto fut = pool.submit([client_fd, this]() {
            this->handleClient(client_fd);
            // return 0;
        });
    }
}

void HTTPServer::handleClient(int client_fd){ // the main logic
    static std::atomic<int> connectionCounter{0};
    int id = ++connectionCounter;
    std::cout << "Connection= " << id << ", Thread ID: " << std::this_thread::get_id()
          << ", FD: " << client_fd << std::endl;
    char buffer[8192] = {0};
    ssize_t bytesRead = read(client_fd, buffer, sizeof(buffer) - 1); // Read at most one less than the buffer size
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Add the null terminator
        std::string request(buffer);
        // ... process the request ...
        if (request.find("GET /favicon.ico") == 0) {
            std::cout << "[info] Ignoring favicon request." << std::endl;
        
            // Send a 204 No Content response to close the request cleanly
            const char* response = "HTTP/1.1 204 No Content\r\n\r\n";
            send(client_fd, response, strlen(response), 0);
            close(client_fd);
            return;
        }
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
    } else if (bytesRead <= 0) {
        // Connection closed by client
    } else {
        // Error occurred during read
        perror("read");
        close(client_fd);
        return;
    }
    close(client_fd);
    std::cout << "[close] Closed FD: " << client_fd << std::endl;
}

