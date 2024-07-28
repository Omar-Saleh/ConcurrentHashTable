#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <functional>
#include "HashBucket.hpp"

constexpr u32 hash_table_size = 13;

namespace celonis {

    template<typename K, typename V, typename HashFnT = std::hash<K>>
    struct ConcurrentHashTable {

        ConcurrentHashTable();
        ~ConcurrentHashTable() {
            delete[] m_hashTable;
        }

        bool get(const K& key);

        void put(const K& key, const V& value);

        void delete(const K& key);

    private:
        std::array<HashBucket<K, V>, hash_table_size> m_hashTable;
    };
}

#endif