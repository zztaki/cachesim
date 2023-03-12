/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-09 17:21:40
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:20:52
 */
#include "replace/min.h"
#include "cache_request.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
bool MINCache::evict() {
    while (isFull()) {
        auto maxReuseDis = nextPos2Key_.begin();
        auto victim = cacheMap_.find(maxReuseDis->second);
        if (victim != cacheMap_.end()) {
            currentSize_ -= victim->second;
            cacheMap_.erase(victim);
        }
        nextPos2Key_.erase(maxReuseDis);
    }
    return true;
}

bool MINCache::lookup(const CacheRequest *req) {
    nextPos2Key_.erase(cur_);
    nextPos2Key_.emplace(req->getNextPos(), req->getKey());
    cur_++;

    if (cacheMap_.count(req->getKey())) {
        return true;
    }
    return false;
}

bool MINCache::insert(const CacheRequest *req) {
    cacheMap_[req->getKey()] = req->getSize();
    currentSize_ += req->getSize();
    if (isFull()) {
        evict();
    }
    return true;
}

void MINCache::makeReusePos(const std::string fileName) {
    std::cerr << "make min profile..." << std::endl;
    std::ifstream fin(fileName);
    std::vector<keyType> keyTrace;
    uint64_t t, size;
    keyType key;

    while (fin >> t >> key >> size) {
        keyTrace.push_back(key);
    }

    // key2LastPos records keyTrace[i]'s first appearance pos in [i + 1, idTrace.size() - 1]
    // if not, the pos is defined as UINT64_MAX
    std::unordered_map<keyType, uint64_t> key2LastPos;

    // nextPosVec[i] recodes keyTrace[i]'s pos of next reuse, if not, the pos is defined as UINT64_MAX
    std::vector<uint64_t> nextPosVec(keyTrace.size(), 0);

    for (int i = keyTrace.size() - 1; i >= 0; i--) {
        keyType currendKey = keyTrace[i];
        auto lastIt = key2LastPos.find(currendKey);
        if (lastIt != key2LastPos.end()) {
            nextPosVec[i] = lastIt->second;
        } else {
            nextPosVec[i] = UINT64_MAX;
        }
        key2LastPos[currendKey] = i;
    }

    fin.clear();
    fin.seekg(0, std::ios::beg);

    std::string minProfile = fileName + ".npt";
    std::string tmpFile = minProfile + ".tmp";
    std::ofstream fout(tmpFile);
    for (uint64_t i = 0; i < keyTrace.size(); i++) {
        fin >> t >> key >> size;
        fout << t << " " << key << " " << size << " " << nextPosVec[i] << std::endl;
    }
    fin.close();
    fout.close();
    if (std::rename(tmpFile.c_str(), minProfile.c_str())) {
        std::cerr << "Exception in renaming file from " << tmpFile << " to " << minProfile
                  << " code: " << std::strerror(errno) << std::endl;
        exit(-1);
    }
}