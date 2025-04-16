#pragma once
#include <memory>

struct cacheValue{
    std::shared_ptr<std::string>  data;
    // std::string *data;
    std::string::size_type len;
};  

struct cacheKey{
    std::shared_ptr<std::string> url;
    // std::string *url;
};

class CacheStrategy{
public:
    CacheStrategy(){};
    virtual ~CacheStrategy() = default; //child class destructor won't run if we don't define parent class destructor

    virtual std::shared_ptr<cacheValue> get(const std::string& key) = 0;
    virtual void put(const std::string& key, const std::string& value) = 0;
};