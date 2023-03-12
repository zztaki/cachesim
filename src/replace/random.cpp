#include "replace/random.h"
#include "cache.h"
#include "cache_object.h"
#include <algorithm>
#include <cstdlib>

bool RandomCache::lookup(const CacheRequest *req) { return cacheMap_.find(req->getKey()) != cacheMap_.end(); }

bool RandomCache::insert(const CacheRequest *req) {
    if (!lookup(req)) {
        CacheObject obj(req);
        cacheVec_.push_back(obj);
        cacheMap_[obj.key_] = true;
        currentSize_ += obj.size_;
        if (isFull()) {
            evict();
        }
        return true;
    }
    return false;
}

bool RandomCache::evict() {
    while (isFull()) {
        uint64_t victimIdx = rand() % cacheVec_.size();
        CacheObject victimObj = cacheVec_[victimIdx];
        std::swap(cacheVec_[victimIdx], cacheVec_.back());
        currentSize_ -= victimObj.size_;
        cacheMap_.erase(victimObj.key_);
        cacheVec_.pop_back();
    }
    return true;
}