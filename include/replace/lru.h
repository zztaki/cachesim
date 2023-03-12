/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 21:08:09
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:48:11
 */
#ifndef LRU_H
#define LRU_H
#include "cache.h"
#include "cache_object.h"
#include "cache_request.h"
#include <cstdint>
#include <list>
#include <unordered_map>

typedef std::list<CacheObject>::iterator ListIteratorType;
typedef std::unordered_map<keyType, ListIteratorType> lruCacheMapType;

class LRUCache : public Cache {
protected:
    friend class S4LRUCache;
    friend class ARCCache;
    friend class LIRSCache;
    friend class DLIRSCache;

    // list for recency order
    // std::list is a container, usually, implemented as a doubly-linked list
    std::list<CacheObject> cacheList_;
    // map to find objects in list
    lruCacheMapType cacheMap_;

    /**
     * @description: promote the hit object into the head of LRU list
     * @param it is the iterator of the requested object in LRU list
     * @return {*}
     * @author: zztaki
     */
    virtual void promote(lruCacheMapType::const_iterator it);

    /**
     * @description: lru insertion policy, insert the requested object as index into the
     * head of LRU list, so will not change currentSize_, called by lirs, dlirs
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert_index(const CacheRequest *req);

    /**
     * @description: lru insertion policy, insert the requested object into the
     * head of LRU list, but not check if full, called by lirs, dlirs
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert_not_evict(const CacheRequest *req);

    /**
     * @description: lru eviction policy, evict the tail object in cache
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict();

    /**
     * @description: evict the specific object, be called by s4lru, lirs...
     * @param req is the pointer to a cache requset
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict(const CacheRequest *req);

    /**
     * @description: evict the the tail object in cache and return, be called by
     * s4lru, lirs, dlirs...
     * @return the evicted object
     * @author: zztaki
     */
    virtual CacheRequest *evict_return();

    /**
     * @description: evict the the tail object as index, so will not change currentSize_, called by lirs,
     * dlirs...
     * @return
     * @author: zztaki
     */
    virtual void evict_index();

    /**
     * @description: evict the requested object as index, so will not change currentSize_, called by lirs,
     * dlirs...
     * @return true if evict successfully
     * @author: zztaki
     */
    virtual bool evict_index(const CacheRequest *req);

    /**
     * @description: reset lru cache's capacity
     * @param {uint64_t} newCapacity
     * @return {*}
     * @author: zztaki
     */
    virtual void setCapacity(uint64_t newCapacity) { capacity_ = newCapacity; }

public:
    LRUCache(uint64_t capacity = 0) : Cache(capacity) {}
    virtual ~LRUCache() {}

    /**
     * @description: check if the requested object is in cache, and will do
     * promotion when the request is a hit
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req);

    /**
     * @description: lru insertion policy, insert the requested object into the
     * head of LRU list
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req);
};

static Factory<LRUCache> factoryLRU("LRU");
#endif