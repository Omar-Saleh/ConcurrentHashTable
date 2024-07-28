#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <functional>
#include "HashBucket.hpp"

constexpr u32 DEFAULT_TABLE_SIZE = 13;

namespace concurrency {

    template<typename K, typename V, typename HashFnT = std::hash<K>>
    struct ConcurrentHashTable {

        explicit ConcurrentHashTable(u32 table_size = DEFAULT_TABLE_SIZE) : m_tableSize(table_size) {
            m_hashTable = new HashBucket<K, V>[m_tableSize];
        };

        ~ConcurrentHashTable() {
            delete[] m_hashTable;
        }

        std::optional<V> get(const K& key) {
            u32 idx = m_hashFn(key) % m_tableSize;
            return m_hashTable[idx].get(key);
        }

        void put(const K& key, const V& value) {
            u32 idx = m_hashFn(key) % m_tableSize;
            m_hashTable[idx].put(key, value);
        }

        void erase(const K& key) {
            u32 idx = m_hashFn(key) % m_tableSize;
            m_hashTable[idx].erase(key);
        }

    private:
        HashBucket<K, V>* m_hashTable;
        HashFnT m_hashFn;
        u32 m_tableSize;
    };
}

#endif