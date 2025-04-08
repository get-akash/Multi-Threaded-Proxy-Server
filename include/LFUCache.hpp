#pragma once
#include<iostream>
#include<mutex>
#include<unordered_map>
#include<list>
#include "CacheStrategy.hpp"

class LFUCache: public CacheStrategy{
private:
    int capacity;
    int minFreq;
    std::unordered_map<int, std::list<std::shared_ptr<cacheKey>>> freqList; // shared_ptr helps in moving the key
    std::unordered_map<std::string, std::pair<std::shared_ptr<cacheValue>, std::list<std::shared_ptr<cacheKey>>::iterator>> cache;
    std::unordered_map<std::string, int> keyFreq;
    std::mutex rwLock;

    void moveToFront(int freq, std::list<std::shared_ptr<cacheKey>>::iterator it);
    void updateMinFreq(int freq);
    void removeLFU();
public:
    LFUCache(int cacheCapacity);
    ~LFUCache() override; //overriding base class' destructor

    std::shared_ptr<cacheValue> get(const std::string& key);
    void put(const std::string& key, const std::string& value);
};