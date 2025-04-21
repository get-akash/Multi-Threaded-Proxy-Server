#pragma once

#include <string>
#include <regex>
#include <sstream>
#include <memory>
#include <curl/curl.h>
#include "./CacheStrategy.hpp"

class HTTPServer;

class ServerUtils{
private:
    HTTPServer* server;
    // size_t write_callback(char* buffer, size_t size, size_t nmemb, std::string* userdata); // nmemb: number of members

public:
    ServerUtils(HTTPServer* _server);
    ~ServerUtils() = default; // Explicitly default destructor

    void handleGET(int client_fd, const std::string& url);
    void handlePOST(int client_fd, const std::string& url, const std::string& body);  // For Future Development
    void handlePUT(int client_fd, const std::string& url, const std::string& body);   // For Future Development
    void handleDELETE(int client_fd, const std::string& url);                         // For Future Development

    std::shared_ptr<std::pair<std::string, std::shared_ptr<std::string>>> extractURLFromRequest(const std::string& request);
    std::shared_ptr<std::string> normalizeURL(const std::string& rawURL);
    bool isValidURL(const std::string& url);
    void sendResponse(int client_fd, std::shared_ptr<cacheValue> body);
    std::shared_ptr<std::string> fetchFromWeb(const std::string& url);
    std::shared_ptr<cacheValue> getCachedOrFetched(const std::string& url);
};