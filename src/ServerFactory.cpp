#include "../include/ServerFactory.hpp"

std::unique_ptr<HTTPServer> ServerFactory::createHTTPServer(const std::string& _cacheType, unsigned int _cacheSize, int port){
    auto cache = createCacheStrategy(_cacheType, _cacheSize);
    try{
        return std::make_unique<HTTPServer>(port, std::move(cache));
    }catch(...){
        throw std::invalid_argument("Invalid port");
    }
}

std::unique_ptr<CacheStrategy> ServerFactory::createCacheStrategy(const std::string& cacheType, unsigned int cacheSize){
    if (cacheType == "LRU"){
        return std::make_unique<LRUCache>(cacheSize);
    }else if (cacheType == "LFU"){
        return std::make_unique<LFUCache>(cacheSize);
    }else{
        throw std::invalid_argument("Invalid cache type");
    }
}