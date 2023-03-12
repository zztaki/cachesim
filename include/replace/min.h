/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-09 17:21:05
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:16:47
 */
#ifndef MIN_H
#define MIN_H
#include "cache.h"
#include "cache_request.h"
#include <cstdint>
#include <functional>
#include <unordered_map>
class MINCache : public Cache {
protected:
    // key->size
    std::unordered_map<keyType, uint64_t> cacheMap_;
    // map object's next reuse distance to key
    std::multimap<uint64_t, uint64_t, std::greater<uint64_t>> nextPos2Key_;
    // the current idx of trace
    uint64_t cur_;

    /**
     * @description: min eviction policy, evict the object that has max next reuse distance in cache
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict();

public:
    MINCache(uint64_t capacity) : Cache(capacity), cur_(0) {}
    virtual ~MINCache() {}

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

    /**
     * @description: the min policy needs trace's information, so thie function will do preparation
     * @return {*}
     * @author: zztaki
     */
    static void makeReusePos(const std::string fileName);
};
static Factory<MINCache> factoryMIN("MIN");
#endif