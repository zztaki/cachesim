/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 20:15:22
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:59:19
 */
#ifndef CACHE_REQUEST_H
#define CACHE_REQUEST_H
#include <cstdint>
#include <iostream>
typedef uint64_t keyType;

// Request information
class CacheRequest {
private:
    keyType key_;      // request object id
    uint64_t size_;    // request size in bytes
    bool flag_;        // demote flag
    uint64_t nextPos_; // just be used in offline algos

public:
    CacheRequest() {}
    CacheRequest(keyType key, uint64_t size, bool flag = false, uint64_t nextPos = 0)
        : key_(key), size_(size), flag_(flag), nextPos_(nextPos) {}
    virtual ~CacheRequest() {}

    /**
     * @description: print request to stdout
     * @return {*}
     * @author: zztaki
     */
    void print() const { std::cout << "key: " << getKey() << ", size: " << getSize() << std::endl; }

    /**
     * @description: reinit the key and size of the requested object
     * @param key is the key of requested object
     * @param size is the size of requested object
     * @return {*}
     * @author: zztaki
     */
    void reinit(keyType key, uint64_t size, bool flag = false, uint64_t nextPos = 0) {
        key_ = key;
        size_ = size;
        flag_ = flag;
        nextPos_ = nextPos;
    }

    /**
     * @description: get the requested object's key
     * @return the requested object's key
     * @author: zztaki
     */
    keyType getKey() const { return key_; }

    /**
     * @description: get the requested object's size in bytes
     * @return the requested object's size in bytes
     * @author: zztaki
     */
    uint64_t getSize() const { return size_; }

    /**
     * @description: get the requested object's next reuse pos, just be called in MINCache
     * @return {*}
     * @author: zztaki
     */
    uint64_t getNextPos() const { return nextPos_; }
};
#endif