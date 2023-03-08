/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 22:36:58
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-08 16:36:48
 */
#include "replace/lru.h"
#include "cache_object.h"
#include "cache_request.h"

bool LRUCache::lookup(const CacheRequest *req) {
    auto it = cacheMap_.find(req->getKey());
    if (it != cacheMap_.end()) {
        promote(it);
        return true;
    }
    return false;
}

void LRUCache::promote(lruCacheMapType::const_iterator it) {
    cacheList_.splice(cacheList_.begin(), cacheList_, it->second);
}

bool LRUCache::insert(const CacheRequest *req) {
    // req.key not in cache, do insertion
    if (!lookup(req)) {
        CacheObject obj(req);
        cacheList_.push_front(obj);
        cacheMap_[obj.key_] = cacheList_.begin();
        currentSize_ += obj.size_;
        if (isFull()) {
            evict();
        }
        return true;
    }
    return false;
}

bool LRUCache::evict() {
    if (isFull()) {
        ListIteratorType victim_iterator = cacheList_.end();
        victim_iterator--;
        currentSize_ -= victim_iterator->size_;
        cacheMap_.erase(victim_iterator->key_);
        cacheList_.erase(victim_iterator);
        return true;
    }
    return false;
}