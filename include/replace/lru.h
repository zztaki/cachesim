/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 21:08:09
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-08 17:04:02
 */
#ifndef LRU_H
#define LRU_H
#include "cache.h"
#include "cache_object.h"
#include "cache_request.h"
#include <list>
#include <unordered_map>

typedef std::list<CacheObject>::iterator ListIteratorType;
typedef std::unordered_map<keyType, ListIteratorType> lruCacheMapType;

class LRUCache : public Cache {
protected:
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
     * @description: lru eviction policy, evict the tail object in cache
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict();

public:
    LRUCache(uint64_t capacity = 0) : Cache(capacity) {}
    virtual ~LRUCache() {}

    /**
     * @description: check if the requested object is in cache
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