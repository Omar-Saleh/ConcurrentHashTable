#include <gtest/gtest.h>
#include <thread>
#include <future>
#include "HashTable.hpp"

using hash_table = concurrency::ConcurrentHashTable<std::string, std::string>;

TEST(SingleThread, SingleInsert) {
    hash_table test(1);
    test.put("Hello", "World");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "World");
}

TEST(SingleThread, InvalidKey) {
    hash_table test(1);
    test.put("Hello", "World");
    EXPECT_FALSE(test.get("NON_EXISTENT").has_value());
}

TEST(SingleThread, SameKeyMultiInsert) {
    hash_table test(1);
    test.put("Hello", "World");
    test.put("Hello", "Concurrent");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "Concurrent");
}

TEST(SingleThread, DeleteNonExistentKey) {
    hash_table test(1);
    test.put("Hello", "World");
    test.erase("NON_EXISTENT");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
}

TEST(SingleThread, DeletedKey) {
    hash_table test(1);
    test.put("Hello", "World");
    test.erase("Hello");
    auto res = test.get("Hello");
    EXPECT_FALSE(res.has_value());
}

TEST(SingleThread, DeletedThenReinserted) {
    hash_table test(1);
    test.put("Hello", "World");
    test.erase("Hello");
    test.put("Hello", "HashTable");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "HashTable");
}

TEST(SingleThread, Eviction) {
    concurrency::HashBucket<std::string, std::string> bucket(2);
    bucket.put("K1", "V1");
    bucket.put("K2", "V2");
    bucket.put("K3", "V3");
    EXPECT_FALSE(bucket.get("K1").has_value());
    EXPECT_TRUE(bucket.get("K2").has_value());
    auto res = bucket.get("K3");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "V3");
}

void multiThreadFunction(hash_table& table, std::string own_key, std::string other_key) {
    table.put(own_key, "ConcurrencyCheck");
    auto res = table.get(own_key);
    EXPECT_TRUE(res.has_value());
    std::this_thread::sleep_for(std::chrono::seconds(1));
    res = table.get(other_key);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "ConcurrencyCheck");
};

void insertKeyWithThread(hash_table& table, std::string own_key) {
    table.put(own_key, "ConcurrencyCheck");
    EXPECT_TRUE(table.get(own_key));
}

void mutiThreadDeleteFromOtherThread(hash_table& table, std::string other_key) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    table.erase(other_key);
};

TEST(MultiThread, ThreadsCanReadFromEachOther) {
    hash_table test(1);
    auto t1 = std::async(std::launch::async, multiThreadFunction, std::ref(test), "T1", "T2");
    auto t2 = std::async(std::launch::async, multiThreadFunction, std::ref(test), "T2", "T1");

    t1.wait();
    t2.wait();

    auto res = test.get("T1");
    EXPECT_TRUE(res.has_value());
    res = test.get("T2");
    EXPECT_TRUE(res.has_value());
}

TEST(MultiThread, ThreadsCanDeleteFromEachOther) {
    hash_table test(1);
    auto t1 = std::async(std::launch::async, insertKeyWithThread, std::ref(test), "T1");
    auto t2 = std::async(std::launch::async, mutiThreadDeleteFromOtherThread, std::ref(test), "T1");

    t1.wait();
    t2.wait();

    auto res = test.get("T1");
    EXPECT_FALSE(res.has_value());
}
