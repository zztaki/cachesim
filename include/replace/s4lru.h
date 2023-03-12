/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-08 16:41:47
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:45:20
 */
#ifndef S4LRU_H
#define S4LRU_H
#include "cache.h"
#include "cache_request.h"
#include "replace/lru.h"
#include <cstdint>
class S4LRUCache : public Cache {
protected:
    LRUCache segments_[4];
    uint64_t segmentSize_;

    /**
     * @description: insert the requested object into the idx segment
     * @param {CacheRequest*} req
     * @param {uint8_t} idx
     * @return {*}
     * @author: zztaki
     */
    virtual void insert_segment(const CacheRequest *req, uint8_t idx);

    /**
     * @description: this will not be called, just for virtual
     * @return {*}
     * @author: zztaki
     */
    virtual bool evict() { return true; }

public:
    S4LRUCache(uint64_t capacity = 0) : Cache(capacity), segmentSize_(capacity / 4) {
        segments_[0] = LRUCache(segmentSize_);
        segments_[1] = LRUCache(segmentSize_);
        segments_[2] = LRUCache(segmentSize_);
        segments_[3] = LRUCache(segmentSize_);
    }
    ~S4LRUCache() {}

    /**
     * @description: get the total size of all objects in cache
     * @return the total size of all objs in cache
     * @author: zztaki
     */
    virtual uint64_t getCurrentSize() const;

    /**
     * @description: check if the requested object is in cache
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req);

    /**
     * @description: s4lru insertion policy, insert the requested object into
     * the head of lowest LRU list
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req);
};

static Factory<S4LRUCache> factoryS4LRU("S4LRU");
#endif