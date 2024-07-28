#ifndef HASH_BUCKET_H_
#define HASH_BUCKET_H_

#include "HashNode.hpp"
#include <cstdint>
#include <utility>
#include <mutex>
#include <shared_mutex>
#include <optional>

using u32 = std::size_t;

namespace concurrency {

    template <typename K, typename V>
    struct HashBucket {

        explicit HashBucket(u32 max_elems = 10) : m_currElems(0), m_capacity(max_elems) {};
        ~HashBucket() {
            std::unique_lock lock(m_mutex);
            HashNode<K, V>* prev;
            HashNode<K, V>* curr_node = m_startNode;
            while (curr_node != nullptr) {
                prev = curr_node;
                curr_node = curr_node->next;
                delete prev;
            }
            curr_node = nullptr;
        }

        std::optional<V> get(const K& key) {
            std::shared_lock lock(m_mutex);
            auto* currNode = m_startNode;
            while (currNode != nullptr) {
                if (currNode->key() == key) {
                    return {currNode->value()};
                }
                currNode = currNode->next;
            }
            return {};
        };

        void put(const K& key, const V& value) {
            HashNode<K, V>* node;
            std::unique_lock lock(m_mutex);
            HashNode<K, V>* prev = nullptr;
            auto* currNode = m_startNode;
            while (currNode != nullptr && currNode->key() != key) {
                prev = currNode;
                currNode = currNode->next;
            }
            if (currNode == nullptr) {
                if (m_capacity == m_currElems) {
                    currNode = m_startNode;
                    prev = nullptr;
                    while (currNode->next != nullptr) {
                        prev = currNode;
                        currNode = currNode->next;
                    }
                    prev->next = nullptr;
                    node = currNode;
                    node->setValue(value);
                    node->setKey(key);
                } else {
                    node = new HashNode(key, value);
                }
            } else {
                if (prev != nullptr) {
                    prev->next = currNode->next;
                }
                node = currNode;
                node->setValue(value);
            }
            if (node != m_startNode) {
                node->next = m_startNode;
                m_startNode = node;
            }
            m_currElems++;
        };

        void erase(const K& key) {
            std::unique_lock lock(m_mutex);
            HashNode<K, V>* prev = nullptr;
            auto* currNode = m_startNode;
            while (currNode != nullptr && currNode->key() != key) {
                prev = currNode;
                currNode = currNode->next;
            }
            if (currNode == nullptr) {
                return;
            } else {
                if (currNode != m_startNode) {
                    prev->next = currNode->next;
                } else {
                    m_startNode = currNode->next;
                }
                delete currNode;
                m_currElems--;
            }
        };

    private:
        HashNode<K, V>* m_startNode = nullptr;
        u32 m_currElems;
        u32 m_capacity;
        mutable std::shared_mutex m_mutex;
    };
}

#endif