/*
 * @Description:
 * @Version: 1.0
 * @Autor: zztaki
 * @Date: 2023-03-07 23:47:03
 * @LastEditors: zztaki
 * @LastEditTime: 2023-03-08 16:33:51
 */
#include "cache.h"
#include "cache_request.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "cachesim traceFile cacheType cacheSizeBytes [cacheParams]"
             << endl;
        return 1;
    }

    const char *fileName = argv[1];
    const string cacheType = argv[2];
    uint64_t cacheSize = stoull(argv[3]);
    unique_ptr<Cache> cachesim = Cache::create_unique(cacheType, cacheSize);

    ifstream infile;
    long long reqs = 0, hits = 0;
    long long t, key, size;

    cerr << "running..." << endl;

    infile.open(fileName);
    CacheRequest *req = new CacheRequest(0, 0);
    while (infile >> t >> key >> size) {
        reqs++;
        req->reinit(key, size);
        if (cachesim->lookup(req)) {
            hits++;
        } else if (cachesim->admit(req)) {
            cachesim->insert(req);
        }
    }
    delete req;

    infile.close();
    cout << cacheType << " " << cacheSize << " " << reqs << " " << hits << " "
         << double(hits) / reqs << endl;

    return 0;
}