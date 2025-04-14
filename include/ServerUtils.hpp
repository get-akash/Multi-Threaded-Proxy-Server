#pragma once

#include <string>
// add imports for util functions

void handleGET(int client_fd, const std::string& url);
void handlePOST(int client_fd, const std::string& url, const std::string& body);  // For Future Development
void handlePUT(int client_fd, const std::string& url, const std::string& body);   // For Future Development
void handleDELETE(int client_fd, const std::string& url);                         // For Future Development

std::string extractURLFromRequest(const std::string& request);
std::string normalizeURL(const std::string& rawURL);
bool isValidURL(const std::string& url);
void sendResponse(int client_fd, const std::string& body);
std::string fetchFromWeb(const std::string& url);
std::string getCachedOrFetched(const std::string& url);