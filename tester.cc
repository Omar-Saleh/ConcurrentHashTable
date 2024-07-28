#include <gtest/gtest.h>
#include "HashTable.hpp"


TEST(SingleThread, SingleInsert) {
    celonis::ConcurrentHashTable<std::string, std::string> test(1);
    test.put("Hello", "World");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "World");
}

TEST(SingleThread, SameKeyMultiInsert) {
    celonis::ConcurrentHashTable<std::string, std::string> test(1);
    test.put("Hello", "World");
    test.put("Hello", "Concurrent");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "Concurrent");
}

TEST(SingleThread, DeletedKey) {
    celonis::ConcurrentHashTable<std::string, std::string> test(1);
    test.put("Hello", "World");
    test.erase("Hello");
    auto res = test.get("Hello");
    EXPECT_FALSE(res.has_value());
}

TEST(SingleThread, DeletedThenReinserted) {
    celonis::ConcurrentHashTable<std::string, std::string> test(1);
    test.put("Hello", "World");
    test.erase("Hello");
    test.put("Hello", "HashTable");
    auto res = test.get("Hello");
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "HashTable");
}

