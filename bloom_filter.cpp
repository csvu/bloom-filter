#include "bloom_filter.h"

BloomFilter* createBloomFilter(std::size_t size, int hash_count) {
    BloomFilter* bloom_filter = new BloomFilter;
    bloom_filter->hash_count = hash_count;
    bloom_filter->filter.resize(size);
    return bloom_filter;
}

void destroyBloomFilter(BloomFilter* bloom_filter) { delete bloom_filter; }

void insertMember(BloomFilter* bloom_filter, const std::string& key) {
    unsigned int hash1 = oneAtATime(key);
    unsigned int hash2 = FNV1A(key);
    for (int i = 0; i < bloom_filter->hash_count; i++) {
        bloom_filter->filter[(hash1 + i * hash2) % bloom_filter->filter.size()] = true;
    }
}

bool isPossiblyMember(BloomFilter* bloom_filter, const std::string& key) {
    unsigned int hash1 = oneAtATime(key);
    unsigned int hash2 = FNV1A(key);
    for (int i = 0; i < bloom_filter->hash_count; i++) {
        if (!bloom_filter->filter[(hash1 + i * hash2) % bloom_filter->filter.size()]) {
            return false;
        }
    }
    return true;
}

void inputByFile(BloomFilter* bloom_filter, const std::string& file_name) {
    std::ifstream inp(file_name);
    std::string temp;
    while (inp >> temp) {
        insertMember(bloom_filter, temp);
    }
    inp.close();
}