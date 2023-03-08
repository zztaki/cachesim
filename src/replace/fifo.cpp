/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-08 16:26:44
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-08 23:15:16
 */
#include "replace/fifo.h"
#include "cache_object.h"
#include "cache_request.h"

bool FIFOCache::lookup(const CacheRequest *req) {
    return cacheMap_.find(req->getKey()) != cacheMap_.end();
}

bool FIFOCache::insert(const CacheRequest *req) {
    if (!lookup(req)) {
        CacheObject obj(req);
        cacheQueue_.push(obj);
        cacheMap_[obj.key_] = true;
        currentSize_ += obj.size_;
        if (isFull()) {
            evict();
        }
        return true;
    }
    return false;
}

bool FIFOCache::evict() {
    while (isFull()) {
        CacheObject victimObj = cacheQueue_.front();
        currentSize_ -= victimObj.size_;
        cacheMap_.erase(victimObj.key_);
        cacheQueue_.pop();
        return true;
    }
    return false;
}