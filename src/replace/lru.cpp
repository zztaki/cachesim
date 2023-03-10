/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 22:36:58
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 17:47:56
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

bool LRUCache::insert_index(const CacheRequest *req) {
    // req.key not in cache, do insertion
    if (!lookup(req)) {
        CacheObject obj(req);
        cacheList_.push_front(obj);
        cacheMap_[obj.key_] = cacheList_.begin();
        return true;
    }
    return false;
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

bool LRUCache::insert_not_evict(const CacheRequest *req) {
    // req.key not in cache, do insertion
    if (!lookup(req)) {
        CacheObject obj(req);
        cacheList_.push_front(obj);
        cacheMap_[obj.key_] = cacheList_.begin();
        currentSize_ += obj.size_;
        return true;
    }
    return false;
}

bool LRUCache::evict() {
    while (isFull()) {
        ListIteratorType victim_iterator = cacheList_.end();
        victim_iterator--;
        currentSize_ -= victim_iterator->size_;
        cacheMap_.erase(victim_iterator->key_);
        cacheList_.erase(victim_iterator);
    }
    return true;
}

bool LRUCache::evict(const CacheRequest *req) {
    if (lookup(req)) {
        // when lookup return true, the object have been moved to the head of
        // LRU list
        ListIteratorType victim_iterator = cacheList_.begin();
        currentSize_ -= victim_iterator->size_;
        cacheMap_.erase(victim_iterator->key_);
        cacheList_.erase(victim_iterator);
        return true;
    }
    return false;
}

CacheRequest *LRUCache::evict_return() {
    if (!cacheList_.empty()) {
        ListIteratorType victim_iterator = cacheList_.end();
        victim_iterator--;
        CacheRequest *req = new CacheRequest(victim_iterator->key_, victim_iterator->size_);
        currentSize_ -= victim_iterator->size_;
        cacheMap_.erase(victim_iterator->key_);
        cacheList_.erase(victim_iterator);
        return req;
    }
    return nullptr;
}

void LRUCache::evict_index() {
    ListIteratorType victim_iterator = cacheList_.end();
    victim_iterator--;
    cacheMap_.erase(victim_iterator->key_);
    cacheList_.erase(victim_iterator);
}

bool LRUCache::evict_index(const CacheRequest *req) {
    if (lookup(req)) {
        // when lookup return true, the object have been moved to the head of
        // LRU list
        ListIteratorType victim_iterator = cacheList_.begin();
        cacheMap_.erase(victim_iterator->key_);
        cacheList_.erase(victim_iterator);
        return true;
    }
    return false;
}