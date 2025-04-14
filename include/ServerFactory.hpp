#pragma once
#include "./HTTPServer.hpp"
#include "./CacheStrategy.hpp"
#include "./LRUCache.hpp"
#include "./LFUCache.hpp"

class ServerFactory
{
public:
    static std::unique_ptr<HTTPServer> createHTTPServer(const std::string& _cacheType, unsigned int _cacheSize, int port);

private:
    static std::unique_ptr<CacheStrategy> createCacheStrategy(const std::string &cacheType, unsigned int cacheSize);
};