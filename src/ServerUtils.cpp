#include "../include/ServerUtils.hpp"

ServerUtils::ServerUtils(HTTPServer* _server): server(_server){}

void ServerUtils::handleGET(int client_fd, const std::string& url){
    std::shared_ptr<cacheValue> response = getCachedOrFetched(url);
    sendResponse(client_fd, response);
}

std::shared_ptr<cacheValue> ServerUtils::getCachedOrFetched(const std::string& url){
    auto cacheValuePtr = server->cache->get(url);
    if(cacheValuePtr != nullptr){
        return cacheValuePtr;
    }
    std::shared_ptr<std::string> result = fetchFromWeb(url);
    server->cache->put(url, *result);
    auto stdResult = std::make_shared<cacheValue>();
    stdResult->data = result;
    stdResult->len = result->size();
    return stdResult;
}

std::shared_ptr<std::pair<std::string,std::shared_ptr<std::string>>> ServerUtils::extractURLFromRequest(const std::string& request){
    std::smatch match;
    std::regex reqRegex(R"((GET|POST|PUT|DELETE) /?\?url=([^\s]+))");

    if (std::regex_search(request, match, reqRegex)) {
        std::string method = match[1];
        std::string rawURL = match[2];
        std::shared_ptr<std::string> normalizedUrl = normalizeURL(rawURL);

        if (!isValidURL(*normalizedUrl)){
            return nullptr;
        }
        auto res = std::make_shared<std::pair<std::string, std::shared_ptr<std::string>>>();
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

std::shared_ptr<std::string> ServerUtils::normalizeURL(const std::string& rawURL){
    if (rawURL.find("http://") == 0 || rawURL.find("https://") == 0)
        return std::make_shared<std::string>(rawURL);
    return std::make_shared<std::string>("https://" + rawURL);
}

void ServerUtils::sendResponse(int client_fd, std::shared_ptr<cacheValue> body){
    std::ostringstream res;
    res << "HTTP/1.1 200 OK\r\nContent-Length: " << body->len
        << "\r\nContent-Type: text/plain\r\n\r\n" << *(body->data);
    send(client_fd, res.str().c_str(), res.str().size(), 0); // .c_str() → Gets a pointer to the underlying char* buffer
}

std::shared_ptr<std::string> ServerUtils::fetchFromWeb(const std::string& url){
    CURL* curl = curl_easy_init();
    std::shared_ptr<std::string> response;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return nullptr;
        }
        curl_easy_cleanup(curl);
        return response;
    }else{
        std::cerr << "curl_easy_init() failed" << std::endl;
        return nullptr;
    }
}

size_t ServerUtils::write_callback(char* buffer, size_t size, size_t nmemb, std::string* response){
    if (response == NULL) return 0;
    size_t total = size * nmemb;
    response->append(buffer, total);
    return total;
}