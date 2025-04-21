#include "../include/LFUCache.hpp"

LFUCache::LFUCache(int cacheCapacity){
    capacity = cacheCapacity;
}
LFUCache::~LFUCache(){
    cache.clear();
    freqList.clear();
    keyFreq.clear();
}

std::shared_ptr<cacheValue> LFUCache::get(const std::string& key){
    std::lock_guard<std::mutex> lock(rwLock);
    
    if(cache.find(key) == cache.end()){
        return nullptr;
    }

    // get iterator from cache
    auto it = cache[key].second;
    // get key's freq f from keyFreq
    int freq = keyFreq[key];

    moveToFront(freq, it);
    updateMinFreq(freq);

    cache[key].second = freqList[freq+1].begin();

    // update keyFreq
    keyFreq[key]++;
    // return value
    return cache[key].first;
}

void LFUCache::put(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(rwLock);

    //create newValue
    auto newVal = std::make_shared<cacheValue>();
    newVal->data = std::make_shared<std::string>(value);
    newVal->len = value.size();

    if(cache.find(key) == cache.end()){
        //false
        if(cache.size() == capacity){
            //removeLFU
        }
        //put the key in front of [1] list in freqList
        auto newKey = std::make_shared<cacheKey>();
        newKey->url = std::make_shared<std::string>(key);
        freqList[1].push_front(std::move(newKey));
        //update minFreq to 1
        minFreq = 1;
        //update the keyFreq
        keyFreq[key] = 1;
        //add to cache with newValue
        cache[key] = {newVal, freqList[1].begin()};
    }else{
        // true
        // get iterator from cache
        auto it = cache[key].second;
        // get key's freq f from keyFreq
        int freq = keyFreq[key];
        // moveToFront(freq, string)
        moveToFront(freq, it);
        // update minFreq
        updateMinFreq(freq);
        // update keyFreq
        keyFreq[key]++;
        // update cache with newValue
        cache[key] = {newVal, freqList[freq+1].begin()};
    }
}

void LFUCache::moveToFront(int freq, std::list<std::shared_ptr<cacheKey>>::iterator it){
    // move that cacheKey to freqList[f+1]
    if(freqList.find(freq+1) != freqList.end()){
        freqList[freq+1].push_front(*it);
    }else{
        std::list<std::shared_ptr<cacheKey>> newList;
        newList.push_front(*it);
        freqList[freq+1] = newList;
    }
    // remove cachekey from freqList[f] and iterator from the list
    freqList[freq].erase(it);
}

void LFUCache::updateMinFreq(int freq){
    // update minFreq
    if (freqList[freq].empty()) {
        freqList.erase(freq);
        if (minFreq == freq) {
            minFreq = freqList.begin()->first; ;  // Update leastFreq only if it was the lowest
        }
    }
}

void LFUCache::removeLFU(){
    // popback from freqList[minFreq]
    auto& targetCacheKey = freqList[minFreq].back();
    auto& targetKey = targetCacheKey->url;
    // delete key from cache
    cache.erase(*targetKey);
    // delete key from keyFreq
    keyFreq.erase(*targetKey);

    freqList[minFreq].pop_back();
}