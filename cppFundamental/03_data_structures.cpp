// =============================================================================
// 03_data_structures.cpp — array, vector, map, set, stack, queue, pair, tuple
// =============================================================================
//
// COMPILE:  g++ -std=c++17 -o ds 03_data_structures.cpp
// RUN:      ./ds
//
// C++ STANDARD LIBRARY (STL) gives you battle-tested containers.
// Choose the right container = faster code and simpler logic.
// =============================================================================

#include <iostream>
#include <array>       // std::array  — fixed-size, stack-allocated
#include <vector>      // std::vector — dynamic-size, heap-allocated
#include <map>         // std::map    — sorted key-value pairs (red-black tree)
#include <unordered_map> // hash map — O(1) avg lookup vs O(log n) for map
#include <set>         // sorted unique values
#include <stack>       // LIFO
#include <queue>       // FIFO
#include <utility>     // std::pair
#include <tuple>       // std::tuple
#include <algorithm>   // std::sort, std::find, std::for_each, etc.
#include <string>

int main()
{
    // ── 1. RAW ARRAY (C-style) ────────────────────────────────────────────────
    // Fixed at compile time. No bounds checking. Prefer std::array instead.
    int raw[5] = {10, 20, 30, 40, 50};
    std::cout << "raw[2] = " << raw[2] << "\n";   // 30

    // ── 2. std::array ────────────────────────────────────────────────────────
    // Like a C array but with .size(), .at() (bounds-checked), iterators.
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::cout << "arr.size() = " << arr.size() << "\n";
    std::cout << "arr.at(1)  = " << arr.at(1)  << "\n";  // throws if OOB

    // Range-based for loop (Python-like "for x in arr")
    for (int val : arr)
        std::cout << val << " ";
    std::cout << "\n";

    // ── 3. std::vector ───────────────────────────────────────────────────────
    // THE most-used container. Grows dynamically. Contiguous in memory.
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    v.push_back(7);         // append
    v.pop_back();           // remove last
    v.insert(v.begin(), 0); // insert 0 at front (O(n) — expensive for front!)
    v.erase(v.begin());     // remove first element

    std::cout << "vector size: " << v.size() << "\n";

    std::sort(v.begin(), v.end());          // sort ascending
    for (int val : v) std::cout << val << " ";
    std::cout << "\n";

    // ── 4. std::map (sorted key → value) ─────────────────────────────────────
    // Order is alphabetical/numeric by key. Use when you need sorted iteration.
    std::map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"]   = 87;
    scores["Charlie"] = 91;

    for (auto& [name, score] : scores)      // structured binding (C++17)
        std::cout << name << " → " << score << "\n";

    // Check existence before accessing
    if (scores.count("Alice"))
        std::cout << "Alice's score: " << scores["Alice"] << "\n";

    // ── 5. std::unordered_map (hash map) ─────────────────────────────────────
    // O(1) average lookup. Use this by default if sorted order is not needed.
    std::unordered_map<std::string, int> freq;
    std::string words[] = {"apple", "banana", "apple", "cherry", "banana", "apple"};
    for (auto& w : words) freq[w]++;        // count word frequencies

    for (auto& [word, count] : freq)
        std::cout << word << ": " << count << "\n";

    // ── 6. std::set (sorted unique values) ───────────────────────────────────
    std::set<int> unique = {5, 3, 8, 3, 1, 5};  // duplicates silently dropped
    for (int x : unique) std::cout << x << " ";  // printed sorted: 1 3 5 8
    std::cout << "\n";

    // ── 7. std::stack (LIFO) ─────────────────────────────────────────────────
    std::stack<int> stk;
    stk.push(10);
    stk.push(20);
    stk.push(30);
    std::cout << "stack top: " << stk.top() << "\n";  // 30
    stk.pop();
    std::cout << "after pop: " << stk.top() << "\n";  // 20

    // ── 8. std::queue (FIFO) ─────────────────────────────────────────────────
    std::queue<std::string> q;
    q.push("first");
    q.push("second");
    q.push("third");
    std::cout << "queue front: " << q.front() << "\n";  // first
    q.pop();
    std::cout << "after pop:   " << q.front() << "\n";  // second

    // ── 9. std::pair ─────────────────────────────────────────────────────────
    std::pair<std::string, int> person = {"Alice", 30};
    std::cout << person.first << " is " << person.second << " years old\n";

    auto p = std::make_pair("Bob", 25);   // type-deduced shorthand
    std::cout << p.first << " is " << p.second << "\n";

    // ── 10. std::tuple (arbitrary number of mixed types) ─────────────────────
    auto record = std::make_tuple("Alice", 95, 3.14);
    std::cout << std::get<0>(record) << " scored " << std::get<1>(record) << "\n";

    auto [tname, tscore, tval] = record;  // structured binding — best way (C++17)
    std::cout << tname << " " << tscore << " " << tval << "\n";

    return 0;
}

// =============================================================================
// WHEN TO USE WHICH CONTAINER
//
//   Need              │  Use
//   ──────────────────┼───────────────────────────────────
//   Fixed size        │  std::array<T,N>
//   Resizable list    │  std::vector<T>       ← default choice
//   Key→Value lookup  │  std::unordered_map   ← O(1) avg
//   Sorted key→Value  │  std::map             ← O(log n)
//   Unique elements   │  std::set / unordered_set
//   LIFO (undo stack) │  std::stack
//   FIFO (task queue) │  std::queue
//   Two values        │  std::pair
//   Mixed-type record │  std::tuple  (or a struct/class)
// =============================================================================
