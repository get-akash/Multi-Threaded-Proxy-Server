#include "../include/ServerUtils.hpp"

void ServerUtils::handleGET(int client_fd, const std::string& url){
    std::shared_ptr<cacheValue> response = getCachedOrFetched(url);
    sendResponse(client_fd, response);
}

std::shared_ptr<cacheValue> ServerUtils::getCachedOrFetched(const std::string& url){
    auto cacheValuePtr = cache->get(url);
    if(cacheValuePtr != nullptr){
        return cacheValuePtr;
    }
    std::shared_ptr<std::string> result = fetchFromWeb(url);
    cache->put(url, *result);
    auto stdResult = std::make_shared<cacheValue>();
    stdResult->data = result;
    stdResult->len = result->length();
    return stdResult;
}

std::shared_ptr<std::pair<std::string, std::string>> ServerUtils::extractURLFromRequest(const std::string& request){
    std::smatch match;
    std::regex reqRegex(R"((GET|POST|PUT|DELETE) /?\?url=([^\s]+))");

    if (std::regex_search(request, match, reqRegex)) {
        std::string method = match[1];
        std::string rawURL = match[2];
        std::string normalizedUrl = normalizeURL(rawURL);

        if (!isValidURL(normalizedUrl)){
            return nullptr;
        }
        auto res = std::make_shared<std::pair<std::string, std::string>>();
        res->first = method;
        res->second = normalizedUrl;
        return res;
    }
    return nullptr;
}

bool ServerUtils::isValidURL(const std::string& url){
    std::regex urlRegex(R"(https?:\/\/([\w\-]+\.)+[\w\-]+(\/[\w\-._~:/?#[\]@!$&'()*+,;=]*)?)");
    return std::regex_match(url, urlRegex);
}