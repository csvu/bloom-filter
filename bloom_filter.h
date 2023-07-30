
#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include "hash_functions.h"

const int FILTER_SIZE = 201978;
const int HASH_COUNT = 7;

struct BloomFilter {
    int hash_count;

    std::vector<bool> filter;
};

BloomFilter* createBloomFilter(std::size_t size, int hash_count);

void destroyBloomFilter(BloomFilter* bloom_filter);

void insertMember(BloomFilter* bloom_filter, const std::string& key);

bool isPossiblyMember(BloomFilter* bloom_filter, const std::string& key);

void inputByFileUsername(BloomFilter* bloom_filter, const std::string& file_name);

void inputByFileWeakPassword(BloomFilter* bloom_filter, const std::string& file_name);