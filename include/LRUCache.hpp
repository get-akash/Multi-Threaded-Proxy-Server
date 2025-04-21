#pragma once
#include<iostream>
#include <mutex>
#include<unordered_map>
#include <memory>
#include<list>
#include "CacheStrategy.hpp"

class LRUCache: public CacheStrategy{
private:
    int capacity;
    std::list<std::unique_ptr<cacheKey>> lruList; // front->MRU back->LRU
    std::unordered_map<std::string, std::pair<std::shared_ptr<cacheValue>, std::list<std::unique_ptr<cacheKey>>::iterator>> cache;
    std::mutex rwLock;

    void moveToFront(std::list<std::unique_ptr<cacheKey>>::iterator it);
    void removeLRU();
public:
    LRUCache(int cacheCapacity);
    ~LRUCache() override; //overriding the base class’ destructor

    std::shared_ptr<cacheValue> get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;
};