/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-09 17:20:42
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-09 21:58:17
 */
#ifndef ARC_H
#define ARC_H
#include "cache.h"
#include "cache_object.h"
#include "cache_request.h"
#include "replace/lru.h"
#include <cstdint>
class ARCCache : public Cache {
protected:
    LRUCache recencyCache_, frequencyCache_, recencyShadowCache_, frequencyShadowCache_;

    /**
     * @description: insert the requested object into the head of frequency lru list, if is full, the tail
     * objects will be removed and inserted to frequency shadow list
     * @param {CacheRequest} *req
     * @return {*}
     * @author: zztaki
     */
    void insert_frequency(const CacheRequest *req);

    /**
     * @description: insert the requested object into the head of recency lru list, if is full, the tail
     * objects will be removed and inserted to recency shadow list
     * @param {CacheRequest} *req
     * @return {*}
     * @author: zztaki
     */
    void insert_recency(const CacheRequest *req);

    /**
     * @description: adjust cache workload by changing the length of recency and frequency lru list
     * @param {uint64_t} recencyCacheNewCapacity
     * @return {*}
     * @author: zztaki
     */
    void adjust(uint64_t recencyCacheNewCapacity);

    /**
     * @description: when recency lru list is shorter, this will be called
     * @return {*}
     * @author: zztaki
     */
    virtual bool evict();

public:
    ARCCache(uint64_t capacity = 0)
        : Cache(capacity), recencyCache_(capacity / 2), frequencyCache_(capacity / 2),
          recencyShadowCache_(capacity / 2), frequencyShadowCache_(capacity / 2) {}
    ~ARCCache() {}

    /**
     * @description: get the total size of all objs in cache
     * @return the total size of all objs in cache
     * @author: zztaki
     */
    virtual uint64_t getCurrentSize() const;

    /**
     * @description: Check if the requested object is in cache
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req);

    /**
     * @description: determine where the requested object is inserted into
     * the cache (be implemented by cache replacement policy)
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req);
};

static Factory<ARCCache> factoryARC("ARC");
#endif