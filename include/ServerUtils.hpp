#pragma once

#include <string>
#include <regex>
#include "./HTTPServer.hpp"
#include "./CacheStrategy.hpp"
// add imports for util functions

class ServerUtils: HTTPServer{
public:
    ServerUtils() = default; // Explicitly default constructor
    ~ServerUtils() = default; // Explicitly default destructor

    void handleGET(int client_fd, const std::string& url);
    void handlePOST(int client_fd, const std::string& url, const std::string& body);  // For Future Development
    void handlePUT(int client_fd, const std::string& url, const std::string& body);   // For Future Development
    void handleDELETE(int client_fd, const std::string& url);                         // For Future Development

    std::shared_ptr<std::pair<std::string, std::string>> extractURLFromRequest(const std::string& request);
    std::string normalizeURL(const std::string& rawURL);
    bool isValidURL(const std::string& url);
    void sendResponse(int client_fd, std::shared_ptr<cacheValue> body);
    std::shared_ptr<std::string> fetchFromWeb(const std::string& url);
    std::shared_ptr<cacheValue> getCachedOrFetched(const std::string& url);
}