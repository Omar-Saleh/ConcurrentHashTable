# ConcurrentHashTable

This was tested using Clang 15, using cmake it should be possible to configure and run tester.cc which contains all the test cases for the project.

- This is a chaining hashtable that uses LRU as an eviction strategy.
- We use locking to support concurrency and either do shared/unique locking depending on the operation.
- Locking is done on the bucket level.