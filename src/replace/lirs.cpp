/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-09 17:21:37
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 17:24:46
 */
#include "replace/lirs.h"
#include "cache_object.h"
#include "cache_request.h"
#include "replace/lru.h"
#include <cstdint>

void LIRSCache::cut() {
    if (lirsStack_.cacheList_.empty()) {
        return;
    }
    uint64_t tailIndexNum = 0;
    ListIteratorType cutEndPoint = lirsStack_.cacheList_.end();
    cutEndPoint--;
    while (cutEndPoint != lirsStack_.cacheList_.begin() && objMap_[cutEndPoint->key_] != LIR) {
        if (objMap_[cutEndPoint->key_] == NONRESHIR) {
            objMap_.erase(cutEndPoint->key_);
        }
        tailIndexNum++;
        cutEndPoint--;
    }
    if (objMap_[cutEndPoint->key_] != LIR) { // if cutEndPoint == lirsStack_.cacheList_.begin()
        if (objMap_[cutEndPoint->key_] == NONRESHIR) {
            objMap_.erase(cutEndPoint->key_);
        }
        tailIndexNum++;
    }
    while (tailIndexNum--) {
        lirsStack_.evict_index();
    }
}

bool LIRSCache::evict() {
    while (lirsStack_.getCurrentSize() > lirsStack_.getCapacity()) {
        const CacheRequest *demoteReq = lirsStack_.evict_return();
        hirQueue_.insert_not_evict(demoteReq);
        objMap_[demoteReq->getKey()] = RESHIR;
        cut();
        delete demoteReq;
    }
    while (hirQueue_.getCurrentSize() > hirQueue_.getCapacity()) {
        const CacheRequest *victimReq = hirQueue_.evict_return();
        if (lirsStack_.cacheMap_.find(victimReq->getKey()) == lirsStack_.cacheMap_.end()) {
            objMap_.erase(victimReq->getKey());
        } else {
            objMap_[victimReq->getKey()] = NONRESHIR;
        }
        delete victimReq;
    }
    return true;
}

bool LIRSCache::lookup(const CacheRequest *req) {
    const keyType reqKey = req->getKey();
    if (objMap_.find(reqKey) != objMap_.end()) {
        if (objMap_[reqKey] == NONRESHIR) {
            return false;
        } else if (objMap_[reqKey] == LIR) { // hit lir object
            lirsStack_.lookup(req);          // promote the obj to the mru position of lirsStack_
            cut();
        } else if (objMap_[reqKey] == RESHIR) { // hit resident hir object
            if (lirsStack_.lookup(req)) {       // hir object's index is in lirsStack_
                lirsStack_.evict_index(req);
                lirsStack_.insert_not_evict(req);
                // lirsStack_.currentSize_ += reqSize;

                hirQueue_.evict(req);
                objMap_[reqKey] = LIR;
                if (lirsStack_.isFull()) {
                    evict();
                }
            } else { // hir object's index is not in lirsStack_
                hirQueue_.lookup(req);
                lirsStack_.insert_index(req);
            }
        }
        return true;
    }
    return false;
}

bool LIRSCache::insert(const CacheRequest *req) {
    const keyType reqKey = req->getKey();
    const uint64_t reqSize = req->getSize();
    if (objMap_.count(reqKey) && objMap_[reqKey] == NONRESHIR) { // cahce miss but its index is in lirsStack_
        lirsStack_.evict_index(req);
        lirsStack_.insert_not_evict(req);
        objMap_[reqKey] = LIR;
        evict();
    }
    // cache miss and its index is not in lirsStack_
    else if (hirQueue_.getCurrentSize() == 0 &&
             lirsStack_.getCurrentSize() + reqSize <= lirsStack_.getCapacity()) { // init lirsStack_
        lirsStack_.insert_not_evict(req);
        objMap_[reqKey] = LIR;
    } else {
        lirsStack_.insert_index(req);
        hirQueue_.insert_not_evict(req);
        objMap_[reqKey] = RESHIR;
        evict();
    }
    return true;
}