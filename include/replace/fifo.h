/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-08 16:18:21
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-09 14:13:50
 */
#ifndef FIFO_H
#define FIFO_H
#include "cache.h"
#include "cache_object.h"
#include "cache_request.h"
#include <queue>
#include <unordered_map>
class FIFOCache : public Cache {
protected:
    // queue for first-in-first-out order
    std::queue<CacheObject> cacheQueue_;

    // fifo needn't promote, so just can check if the object is in cache in O(1)
    // and save space
    std::unordered_map<keyType, bool> cacheMap_;

    /**
     * @description: fifo eviction policy, evict the first-in object in
     * cache
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict();

public:
    FIFOCache(uint64_t capacity = 0) : Cache(capacity) {}
    virtual ~FIFOCache() {}

    /**
     * @description: check if the requested object is in cache
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req);

    /**
     * @description: fifo insertion policy, insert the requested object into the
     * tail of fifo queue
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req);
};

static Factory<FIFOCache> factoryFIFO("FIFO");
#endif