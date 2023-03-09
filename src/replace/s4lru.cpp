/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-08 17:03:04
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-09 16:19:23
 */
#include "replace/s4lru.h"
#include "cache_request.h"

bool S4LRUCache::lookup(const CacheRequest *req) {
    if (segments_[0].lookup(req)) {
        return true;
    } else if (segments_[1].lookup(req)) {
        segments_[1].evict(req);
        insert_segment(req, 0);
        return true;
    } else if (segments_[2].lookup(req)) {
        segments_[2].evict(req);
        insert_segment(req, 1);
        return true;
    } else if (segments_[3].lookup(req)) {
        segments_[3].evict(req);
        insert_segment(req, 2);
        return true;
    }
    return false;
}

void S4LRUCache::insert_segment(const CacheRequest *req, uint8_t idx) {
    if (idx == 3) {
        segments_[idx].insert(req);
        return;
    }
    while (segments_[idx].getCurrentSize() + req->getSize() > segmentSize_) {
        const CacheRequest *demote_req = segments_[idx].evict_return();
        insert_segment(demote_req, idx + 1);
        delete demote_req;
    }
    segments_[idx].insert(req);
}

bool S4LRUCache::insert(const CacheRequest *req) {
    if (!lookup(req) && req->getSize() <= segmentSize_) {
        insert_segment(req, 3);
        return true;
    }
    return false;
}
