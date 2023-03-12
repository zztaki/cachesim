/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-09 17:20:49
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:54:45
 */
#ifndef LIRS_H
#define LIRS_H
#include "cache.h"
#include "cache_request.h"
#include "replace/lru.h"
#include <unordered_map>

class LIRSCache : public Cache {
protected:
    // lirsStack_ stores lir objects and index of objects that may be lir
    // hirQueue_ stores resident hir objects
    LRUCache lirsStack_, hirQueue_;
    enum stateType {
        LIR = 0,
        RESHIR,
        NONRESHIR,
    };
    // map object's key to its state
    std::unordered_map<keyType, stateType> objMap_;

    /**
     * @description: make sure that lirsStack_ and hirQueue_ is not full
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict();

    /**
     * @description: lirsStack_ cut, keep the bottom object is lir
     * @return {*}
     * @author: zztaki
     */
    void cut();

public:
    LIRSCache(uint64_t capacity = 0)
        : Cache(capacity), lirsStack_(0.99 * capacity), hirQueue_(0.01 * capacity) {}
    virtual ~LIRSCache() {}

    /**
     * @description: get the total size of all objs in cache
     * @return the total size of all objs in cache
     * @author: zztaki
     */
    virtual uint64_t getCurrentSize() const {
        return lirsStack_.getCurrentSize() + hirQueue_.getCurrentSize();
    }

    /**
     * @description: Check if the requested object is in cache, the specific implementation is complex...
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req);

    /**
     * @description: insert the requested object into dlirs cache, the specific implementation is complex...)
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req);
};

static Factory<LIRSCache> factoryLIRS("LIRS");
#endif