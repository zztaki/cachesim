/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 19:33:44
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 17:31:12
 */
#ifndef CACHE_H
#define CACHE_H
#include "cache_request.h"
#include <cstdint>
#include <map>
#include <memory>

class Cache;

class CacheFactory {
public:
    CacheFactory() {}
    virtual std::unique_ptr<Cache> create_unique(uint64_t capacity) = 0;
};

class Cache {
protected:
    uint64_t capacity_;    // size of cache in bytes
    uint64_t currentSize_; // total size of all objects int cache in bytes

    // helper functions (factory pattern)
    static std::map<std::string, CacheFactory *> &get_factory_instance() {
        static std::map<std::string, CacheFactory *> map_instance;
        return map_instance;
    }

    /**
     * @description: check if cache is full
     * @return true if cache is full, else return false
     * @author: zztaki
     */
    bool isFull() const { return currentSize_ > capacity_; }

    /**
     * @description: determine which object to be evicted from the cache (be
     * implemented by cache replacement policy)
     * @param req is the pointer to a cache requset
     * @return true if evict successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool evict() = 0;

public:
    Cache(uint64_t capacity = 0) : capacity_(capacity), currentSize_(0) {}
    virtual ~Cache() {}

    // helper functions (factory pattern)
    static void registerType(std::string name, CacheFactory *factory) {
        get_factory_instance()[name] = factory;
    }
    static std::unique_ptr<Cache> create_unique(std::string name, uint64_t capacity) {
        std::unique_ptr<Cache> Cache_instance;
        if (get_factory_instance().find(name) == get_factory_instance().end()) {
            std::cerr << "unkown cacheType" << std::endl;
            return nullptr;
        }
        Cache_instance = get_factory_instance()[name]->create_unique(capacity);
        return Cache_instance;
    }

    /**
     * @description: get cache's capacity
     * @return cache's capacity
     * @author: zztaki
     */
    virtual uint64_t getCapacity() const { return capacity_; }

    /**
     * @description: get the total size of all objs in cache
     * @return the total size of all objs in cache
     * @author: zztaki
     */
    virtual uint64_t getCurrentSize() const { return currentSize_; }

    /**
     * @description: Check if the requested object is in cache
     * @param req is the pointer to a cache requset
     * @return true if the requested object is in cache, else return false
     * @author: zztaki
     */
    virtual bool lookup(const CacheRequest *req) = 0;

    /**
     * @description: determine whether the requested object is cached
     * @param req is the pointer to a cache requset
     * @return true if the requested object is admitted into cache, else
     * return false (default return true, else be implemented by cache
     * admission policy)
     * @author: zztaki
     */
    virtual bool admit(const CacheRequest *req) const { return true; }

    /**
     * @description: determine where the requested object is inserted into
     * the cache (be implemented by cache replacement policy)
     * @param req is the pointer to a cache requset
     * @return true if insert successfully, else print log and return false
     * @author: zztaki
     */
    virtual bool insert(const CacheRequest *req) = 0;
};

template <class T> class Factory : public CacheFactory {
public:
    Factory(std::string name) { Cache::registerType(name, this); }
    std::unique_ptr<Cache> create_unique(uint64_t capacity) {
        std::unique_ptr<Cache> newT = std::make_unique<T>(capacity);
        return newT;
    }
};

#endif