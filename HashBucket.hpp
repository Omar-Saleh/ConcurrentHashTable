#ifndef HASH_BUCKET_H_
#define HASH_BUCKET_H_

#include "HashNode.hpp"
#include <cstdint>
#include <utility>
#include <mutex>
#include <shared_mutex>
#include <optional>

using u32 = std::size_t;
constexpr u32 MAX_BUCKET_SIZE = 10;

namespace celonis {

    template <typename K, typename V>
    struct HashBucket {

        HashBucket() : m_capacity(0) {};

        std::optional<V> get(const K& key);

        void put(const K& key, const V& value);

        void erase(const K& key);

    private:
        HashNode<K, V>* m_startNode = nullptr;
        u32 m_capacity;
        mutable std::shared_mutex m_mutex;
    };
}

#endif