#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <vector>

struct BloomFilter {
    std::size_t size;
    int hash_count;

    bool* filter;
};

void initSystem(std::size_t expected_users, std::size_t filter_size);

BloomFilter* bloomFilterFromFile(const std::string& filename);

BloomFilter* createBloomFilter(std::size_t size, int hash_count);

void destroyBloomFilter(BloomFilter* bloom_filter);

void insertMember(BloomFilter* bloom_filter, const std::string& key);

bool isMember(BloomFilter* bloom_filter, const std::string& key);

void saveBloomFilter(BloomFilter* bloom_filter, const std::string& filename);
