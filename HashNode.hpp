#ifndef HASH_NODE_H_
#define HASH_NODE_H_

namespace celonis {
    template <typename K, typename V>
    struct HashNode {

        HashNode();
        HashNode(K k, V v) : m_key(k), m_val(v) {}
        ~HashNode() {
            next = nullptr;
        }

        HashNode* next = nullptr;

        const K& key() const {
            return m_key;
        }

        const V& value() const {
            return m_val;
        }

        void setValue(V v) {
            m_val = v;
        }

    private:
        K m_key;
        V m_val;
    };
}

#endif