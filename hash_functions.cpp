#include "hash_functions.h"

unsigned int oneAtATime(const std::string& key) {
    std::size_t i = 0;
    unsigned hash = 0;
    while (i != key.length()) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

unsigned int FNV1A(const std::string& key) {
    unsigned int hash = 2166136261;
    for (std::size_t i = 0; i < key.length(); i++) {
        hash ^= key[i];
        hash *= 16777619;
    }
    return hash;
}