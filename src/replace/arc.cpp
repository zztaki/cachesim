/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-09 17:21:24
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-09 21:59:55
 */
#include "replace/arc.h"
#include "cache_request.h"
#include "replace/lru.h"
#include <algorithm>
#include <cstdint>

uint64_t ARCCache::getCurrentSize() const {
    return recencyCache_.getCurrentSize() + frequencyCache_.getCurrentSize();
}

void ARCCache::insert_frequency(const CacheRequest *req) {
    if (req->getSize() > frequencyCache_.capacity_) {
        return;
    }
    while (frequencyCache_.getCurrentSize() + req->getSize() > frequencyCache_.getCapacity()) {
        // if frequence lru list is full, the tail obj of frequence lru list will be demoted into the head
        // of recency lru list
        const CacheRequest *victim_req = frequencyCache_.evict_return();
        frequencyShadowCache_.insert(victim_req);
        delete victim_req;
    }
    // move the hit obj into the head of frequence lru list
    frequencyCache_.insert(req);
}

void ARCCache::insert_recency(const CacheRequest *req) {
    if (req->getSize() > recencyCache_.capacity_) {
        return;
    }
    while (recencyCache_.getCurrentSize() + req->getSize() > recencyCache_.getCapacity()) {
        // if frequence lru list is full, the tail obj of frequence lru list will be demoted into the head
        // of recency lru list
        const CacheRequest *victim_req = recencyCache_.evict_return();
        recencyShadowCache_.insert(victim_req);
        delete victim_req;
    }
    // move the hit obj into the head of frequence lru list
    recencyCache_.insert(req);
}

void ARCCache::adjust(uint64_t recencyCacheNewCapacity) {
    recencyCache_.setCapacity(recencyCacheNewCapacity);
    recencyShadowCache_.setCapacity(capacity_ - recencyCacheNewCapacity);
    frequencyCache_.setCapacity(capacity_ - recencyCacheNewCapacity);
    frequencyShadowCache_.setCapacity(recencyCacheNewCapacity);
}

bool ARCCache::lookup(const CacheRequest *req) {
    if (frequencyCache_.lookup(req)) { // hit frequence lru list
        // lookup has completed promotion
    } else if (recencyCache_.lookup(req)) { // hit recency lru list
        recencyCache_.evict(req);
        insert_frequency(req);
    } else if (recencyShadowCache_.lookup(req)) { // hit recency shadow list
        uint64_t recencyCacheNewCapacity =
            std::min(recencyCache_.capacity_ + std::max(req->getSize(), frequencyShadowCache_.capacity_ /
                                                                            recencyShadowCache_.capacity_),
                     capacity_ - 1);
        adjust(recencyCacheNewCapacity);
        recencyShadowCache_.evict(req);
        insert_frequency(req);
    } else if (frequencyShadowCache_.lookup(req)) { // hit frequence shadow list
        uint64_t recencyCacheNewCapacity =
            std::max(recencyCache_.capacity_ - std::max(req->getSize(), recencyShadowCache_.capacity_ /
                                                                            frequencyShadowCache_.capacity_),
                     (unsigned long)1);
        adjust(recencyCacheNewCapacity);
        frequencyShadowCache_.evict(req);
        insert_frequency(req);
        evict();
    } else {
        return false;
    }
    return true;
}

bool ARCCache::evict() {
    while (recencyCache_.isFull()) {
        const CacheRequest *victim_req = recencyCache_.evict_return();
        recencyShadowCache_.insert(victim_req);
        delete victim_req;
    }
    return true;
}

bool ARCCache::insert(const CacheRequest *req) {
    if (!lookup(req)) {
        insert_recency(req);
        return true;
    }
    return false;
}