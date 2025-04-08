#include "../include/LRUCache.hpp"

LRUCache::LRUCache(int cacheCapacity){
    capacity = cacheCapacity;
};

LRUCache::~LRUCache(){
    lruList.clear();
    cache.clear();
}

std::shared_ptr<cacheValue> LRUCache::get(const std::string& key){
    std::unique_lock<std::mutex> lock(rwLock);

    if(cache.find(key) == cache.end()){
        return nullptr;
    }

    auto it = cache[key].second;
    moveToFront(it);
    
    cache[key].second = lruList.begin();
    return cache[key].first; //returning pointer to avoid copying in the return value
}

void LRUCache::put(const std::string& key, const std::string& value){
    std::unique_lock<std::mutex> lock(rwLock);

    auto newVal = std::make_shared<cacheValue>();
    newVal->data = std::make_shared<std::string>(value);
    newVal->len = value.length();

    if(cache.find(key) == cache.end()){
        //not on the cache
        if(lruList.size() == capacity){
            removeLRU();
        }

        //add to front on lruList
        auto newKey = std::make_unique<cacheKey>();
        newKey->url = std::make_shared<std::string>(key);
        lruList.push_front(newKey);
        //add the value on cache
        auto newPair = std::pair(newVal, lruList.begin());
        cache[key] = newPair;
    }else{
        //already on the cache
        //move to front of lrulist
        auto it = cache[key].second;
        moveToFront(it);
        //update value on cache 
        cache[key].first = newVal;
    }
};

void LRUCache::moveToFront(std::list<std::unique_ptr<cacheKey>>::iterator it){
     lruList.splice(lruList.begin(), lruList, it);
};

void LRUCache::removeLRU(){
    auto& lruKey = lruList.back();
    auto& key = lruKey->url;
    lruList.pop_back();
    cache.erase(*key);
};