/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 22:20:19
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-08 16:38:48
 */
#ifndef CACHE_OBJECT_H
#define CACHE_OBJECT_H
#include "cache_request.h"

// CacheObject is used by caching policies to store a representation of an
// "object, i.e., the object's id and its size
struct CacheObject {
    keyType key_;
    uint64_t size_;

    CacheObject(const CacheRequest *req)
        : key_(req->getKey()), size_(req->getSize()) {}

    // comparison is based on all three properties
    bool operator==(const CacheObject &rhs) const {
        return (rhs.key_ == key_) && (rhs.size_ == size_);
    }
};
#endif