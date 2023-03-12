/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 23:47:03
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-12 22:36:30
 */
#include "cache.h"
#include "cache_request.h"
#include "replace/min.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "cachesim traceFile cacheType cacheSizeBytes [cacheParams]" << endl;
        return 1;
    }

    srand((unsigned)time(NULL));

    bool is_offline = false;
    const char *fileName = argv[1];
    const string cacheType = argv[2];
    uint64_t cacheSize = stoull(argv[3]);
    unique_ptr<Cache> cachesim = Cache::create_unique(cacheType, cacheSize);
    if (cacheType == "MIN") {
        is_offline = true;
        string minProfile = argv[1];
        minProfile += ".npt";
        ifstream f(minProfile);
        if (!f.good()) {
            MINCache::makeReusePos(fileName);
        }
        fileName = minProfile.c_str();
    }

    ifstream infile;
    long long reqs = 0, hits = 0;
    uint64_t t, size, nextPos;
    keyType key;

    cerr << "running..." << endl;

    infile.open(fileName);
    CacheRequest *req = new CacheRequest(0, 0);
    while (infile >> t >> key >> size) {
        reqs++;
        if (is_offline) {
            infile >> nextPos;
            req->reinit(key, size, false, nextPos);
        } else {
            req->reinit(key, size);
        }

        if (cachesim->lookup(req)) {
            hits++;
        } else if (cachesim->admit(req)) {
            cachesim->insert(req);
        }
    }
    delete req;

    infile.close();
    cout << "cacheType: " << cacheType << ", cacheSize: " << cacheSize << " Bytes, #req: " << reqs
         << ", #hit: " << hits << ", hit ratio: " << double(hits) / reqs << endl;

    return 0;
}