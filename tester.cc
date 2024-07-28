#include <gtest/gtest.h>
#include <thread>
#include <future>
#include "HashTable.hpp"

using hash_table = celonis::ConcurrentHashTable<std::string, std::string>;

TEST(SingleThread, SingleInsert) {
    hash_table test(1);
    test.put("Hello", "World");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "World");
}

TEST(SingleThread, SameKeyMultiInsert) {
    hash_table test(1);
    test.put("Hello", "World");
    test.put("Hello", "Concurrent");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "Concurrent");
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
