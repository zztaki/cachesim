/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-08 17:07:35
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-09 14:22:49
 */
#ifndef RANDOM_H
#define RANDOM_H
#include "cache.h"
#include "cache_object.h"
#include "cache_request.h"
#include <cstdint>
#include <unordered_map>
#include <vector>
class RandomCache : public Cache {
protected:
    std::vector<CacheObject> cacheVec_;
    std::unordered_map<keyType, bool> cacheMap_;

    /**
     * @description: random eviction policy, evict the random object in cache
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict();

public:
    RandomCache(uint64_t capacity = 0) : Cache(capacity) {}
    virtual ~RandomCache() {}

    /**
     * @description: check if the requested object is in cache
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req);

    /**
     * @description: random insertion policy, insert the requested object into
     * the back of random vector
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req);
};

static Factory<RandomCache> factoryRandom("RANDOM");
#endif