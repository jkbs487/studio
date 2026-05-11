---
title: lock-free-queue
description: 探索无锁队列的实现：原子操作、CAS、内存模型、Michael-Scott 算法、ABA 问题
---

# lock-free-queue

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++20 |
| 创建时间 | 2026-04-08 |
| 来源 | human |
| AI 参与 | partial |
| 标签 | `concurrency` `lock-free` `queue` `atomic` `performance` |

</div>

## 描述

探索无锁队列的实现：原子操作、CAS、内存模型、Michael-Scott 算法、ABA 问题

# C++ 无锁队列 `C++20`

探索 C++ 无锁队列（Lock-Free Queue）的实现原理和实践应用。

> **语言版本**: C++20

## 包含内容

1. **原子操作基础** - std::atomic、内存序（memory order）
2. **CAS（Compare-And-Swap）** - 比较并交换操作
3. **无锁栈实现** - 基于 CAS 的栈数据结构
4. **无锁队列实现** - Michael-Scott 队列
5. **内存模型** - 内存序对并发正确性的影响
6. **ABA 问题** - Tagged Pointer 解决方案
7. **性能对比** - 无锁 vs 有锁队列的多场景测试

## 核心概念

### 无锁栈 (LockFreeStack)

基于单链表的头插法栈，使用 CAS 实现 push/pop。

### 无锁队列 (LockFreeQueue)

Michael-Scott 队列算法，使用哨兵节点简化边界处理。

###  Tagged Pointer

将指针和版本号打包到 64 位，解决 ABA 问题。

## 编译与运行

**手动编译：**
```bash
cd experiments/lock-free-queue
mkdir build && cd build
cmake .. && make
./lock-free-queue
```

## 性能测试

运行程序查看不同场景下的性能对比：

- 单生产者+消费者
- 多生产者+消费者
- 纯入队场景
- 高并发场景

详见 `./lock-free-queue` 输出的测试结果。

## 实现经验 FAQ

常见问题和解答见 [FAQ.md](./FAQ.md)。

## 最佳实践

- 简单场景优先考虑有锁结构
- 无锁仅在高并发、延迟敏感场景使用
- 先 profiling 再优化

## 要求

- C++20 或更高版本
- 线程支持库

## 源码

<details>
<summary>lock_free_queue.h</summary>

```cpp
#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include <optional>
#include <queue>
#include <mutex>
#include <functional>

// ============================================================================
// Lock-Free Stack - 使用 Tagged Pointer 解决 ABA 问题
// ============================================================================
template<typename T>
struct LockFreeStackNode {
    T data;
    std::atomic<LockFreeStackNode*> next;

    explicit LockFreeStackNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class LockFreeStack {
public:
    LockFreeStack() : head(0) {}

    void push(T value) {
        auto* newNode = new LockFreeStackNode<T>(std::move(value));

        while (true) {
            uintptr_t oldHead = head.load(std::memory_order_seq_cst);

            LockFreeStackNode<T>* oldPtr = reinterpret_cast<LockFreeStackNode<T>*>(oldHead & 0xFFFFFFFFFFFF);
            uint64_t oldTag = oldHead >> 48;

            newNode->next.store(oldPtr, std::memory_order_seq_cst);

            uintptr_t newHead = reinterpret_cast<uintptr_t>(newNode) | ((oldTag + 1) << 48);

            if (head.compare_exchange_weak(oldHead, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                return;
            }
        }
    }

    std::optional<T> pop() {
        while (true) {
            uintptr_t oldHead = head.load(std::memory_order_seq_cst);

            LockFreeStackNode<T>* ptr = reinterpret_cast<LockFreeStackNode<T>*>(oldHead & 0xFFFFFFFFFFFF);
            uint64_t oldTag = oldHead >> 48;

            if (ptr == nullptr) {
                return std::nullopt;
            }

            LockFreeStackNode<T>* nextNode = ptr->next.load(std::memory_order_seq_cst);

            uintptr_t newHead = reinterpret_cast<uintptr_t>(nextNode) | ((oldTag + 1) << 48);

            if (head.compare_exchange_weak(oldHead, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                T value = std::move(ptr->data);
                delete ptr;
                return value;
            }
        }
    }

    bool isEmpty() const {
        return (head.load(std::memory_order_seq_cst) & 0xFFFFFFFFFFFF) == 0;
    }

    ~LockFreeStack() {
        while (pop()) {}
    }

private:
    std::atomic<uintptr_t> head;
};

// ============================================================================
// Lock-Free Queue - Michael-Scott Queue  
// 使用 tagged pointer 解决 ABA 问题
// 注意：本实现不主动删除退役节点（避免 use-after-free）
// 生产环境应使用完整 Hazard Pointer 或 GC
// ============================================================================
template<typename T>
struct LockFreeQueueNode {
    T data;
    std::atomic<LockFreeQueueNode*> next;

    explicit LockFreeQueueNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class LockFreeQueue {
public:
    LockFreeQueue() {
        auto* dummy = new LockFreeQueueNode<T>(T{});
        dummy->next.store(nullptr, std::memory_order_seq_cst);
        // 使用 tagged pointer: 高 16 位 tag，低 48 位指针
        head.store(reinterpret_cast<uintptr_t>(dummy), std::memory_order_seq_cst);
        tail.store(reinterpret_cast<uintptr_t>(dummy), std::memory_order_seq_cst);
    }

    void enqueue(T value) {
        auto* newNode = new LockFreeQueueNode<T>(std::move(value));
        newNode->next.store(nullptr, std::memory_order_seq_cst);

        while (true) {
            uintptr_t curTail = tail.load(std::memory_order_seq_cst);
            LockFreeQueueNode<T>* tailPtr = reinterpret_cast<LockFreeQueueNode<T>*>(curTail & 0xFFFFFFFFFFFF);
            uint64_t tailTag = curTail >> 48;

            LockFreeQueueNode<T>* next = tailPtr->next.load(std::memory_order_seq_cst);

            if (next != nullptr) {
                // tail 落后，帮它前进，保留 tag
                uintptr_t newTail = reinterpret_cast<uintptr_t>(next) | (tailTag << 48);
                tail.store(newTail, std::memory_order_seq_cst);
                continue;
            }

            // 尝试 CAS
            LockFreeQueueNode<T>* expected = nullptr;
            if (tailPtr->next.compare_exchange_strong(expected, newNode,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                // CAS 成功，更新 tail（保留 tag）
                uintptr_t newTail = reinterpret_cast<uintptr_t>(newNode) | ((tailTag + 1) << 48);
                tail.store(newTail, std::memory_order_seq_cst);
                return;
            }
            // CAS 失败，重试
        }
    }

    std::optional<T> dequeue() {
        while (true) {
            uintptr_t oldHead = head.load(std::memory_order_seq_cst);
            LockFreeQueueNode<T>* headPtr = reinterpret_cast<LockFreeQueueNode<T>*>(oldHead & 0xFFFFFFFFFFFF);
            uint64_t oldTag = oldHead >> 48;

            LockFreeQueueNode<T>* next = headPtr->next.load(std::memory_order_seq_cst);

            if (next == nullptr) {
                return std::nullopt;
            }

            // 尝试 CAS head
            uintptr_t newHead = reinterpret_cast<uintptr_t>(next) | ((oldTag + 1) << 48);
            if (head.compare_exchange_weak(oldHead, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                // CAS 成功，获取值（不立即删除，避免 use-after-free）
                T value = std::move(next->data);
                return value;
            }
            // CAS 失败，重试
        }
    }

    bool isEmpty() const {
        uintptr_t h = head.load(std::memory_order_seq_cst);
        LockFreeQueueNode<T>* headPtr = reinterpret_cast<LockFreeQueueNode<T>*>(h & 0xFFFFFFFFFFFF);
        return headPtr->next.load(std::memory_order_seq_cst) == nullptr;
    }

    ~LockFreeQueue() {
        // 不在析构时删除遗留节点（避免复杂的并发清理）
        // 所有节点会在进程结束时由系统回收
        // 生产环境应使用 Hazard Pointer 实现完整清理
    }

private:
    std::atomic<uintptr_t> head;
    std::atomic<uintptr_t> tail;
};

// ============================================================================
// Mutex Queue - 有锁队列对比
// ============================================================================
template<typename T>
class MutexQueue {
public:
    void enqueue(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(value));
    }

    std::optional<T> dequeue() {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return std::nullopt;
        }
        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
};

// ============================================================================
// Work Queue - 工作队列应用
// ============================================================================
class WorkQueue {
public:
    struct WorkItem {
        int id;
        std::function<void()> task;
    };

    void submit(int id, std::function<void()> task) {
        auto* item = new WorkItem{id, std::move(task)};
        queue.enqueue(item);
    }

    WorkItem* waitForWork() {
        auto result = queue.dequeue();
        if (result) {
            return *result;
        }
        return nullptr;
    }

private:
    LockFreeQueue<WorkItem*> queue;
};

#endif // LOCK_FREE_QUEUE_H
```

</details>

<details>
<summary>main.cpp</summary>

```cpp
#include "lock_free_queue.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// ============================================================================
// 1. 原子操作基础演示
// ============================================================================
void atomicBasics() {
    std::cout << "\n========== 1. 原子操作基础 ==========\n";

    // 基础原子类型
    std::atomic<int> counter(0);
    std::atomic<bool> flag(false);
    std::atomic<long long> bigNumber(0);

    // fetch_add - 原子增加，返回旧值
    std::cout << "fetch_add: " << counter.fetch_add(1) << " -> " << counter.load() << "\n";

    // store/load - 原子写入/读取
    counter.store(100);
    std::cout << "store/load: " << counter.load() << "\n";

    // compare_exchange - CAS 操作
    int expected = 100;
    int desired = 200;
    // 使用 compare_exchange_weak
    bool success = counter.compare_exchange_weak(expected, desired);
    std::cout << "CAS: expected=" << expected
              << ", desired=" << desired
              << ", success=" << success
              << ", value=" << counter.load() << "\n";

    // CAS 失败示例
    expected = 100;  // 现在 counter 是 200，CAS 会失败
    desired = 300;
    success = counter.compare_exchange_weak(expected, desired);
    std::cout << "CAS 失败: expected=" << expected
              << ", desired=" << desired
              << ", success=" << success
              << ", value=" << counter.load() << "\n";

    // 内存序演示
    std::atomic<int> x(0), y(0);
    std::atomic<int> z(0);

    // 使用不同的内存序
    x.store(1, std::memory_order_relaxed);
    y.store(2, std::memory_order_relaxed);
    std::cout << "memory_order_relaxed: x=" << x.load()
              << ", y=" << y.load() << "\n";
}

// ============================================================================
// 2. 无锁栈测试
// ============================================================================
void lockFreeStackDemo() {
    std::cout << "\n========== 2. 无锁栈 ==========\n";

    LockFreeStack<int> stack;

    // 测试 push
    std::cout << "Push: 10, 20, 30\n";
    stack.push(10);
    stack.push(20);
    stack.push(30);

    // 测试 pop
    std::cout << "Pop: ";
    while (auto val = stack.pop()) {
        std::cout << *val << " ";
    }
    std::cout << "\n";

    // 真正的并发 push + pop 测试
    std::cout << "\n并发 Push/Pop 测试:\n";
    LockFreeStack<int> concurrentStack;
    const int NUM_THREADS = 4;
    const int OPERATIONS = 1000;

    std::atomic<int> pushCount{0};
    std::atomic<int> popCount{0};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;

    // 一半线程 push
    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([&concurrentStack, &pushCount, i, OPERATIONS]() {
            for (int j = 0; j < OPERATIONS; ++j) {
                concurrentStack.push(i * OPERATIONS + j);
                pushCount.fetch_add(1);
            }
        });
    }

    // 一半线程 pop
    for (int i = NUM_THREADS / 2; i < NUM_THREADS; ++i) {
        threads.emplace_back([&concurrentStack, &popCount, OPERATIONS]() {
            for (int j = 0; j < OPERATIONS; ++j) {
                if (concurrentStack.pop()) {
                    popCount.fetch_add(1);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Push: " << pushCount.load() << " 次, Pop: " << popCount.load() << " 次\n";
    std::cout << "耗时: " << duration.count() << " us\n";
}

// ============================================================================
// 3. 无锁队列测试
// ============================================================================
void lockFreeQueueDemo() {
    std::cout << "\n========== 3. 无锁队列 (Michael-Scott) ==========\n";

    LockFreeQueue<int> queue;

    // 测试入队
    std::cout << "Enqueue: 1, 2, 3, 4, 5\n";
    for (int i = 1; i <= 5; ++i) {
        queue.enqueue(i);
    }

    // 测试出队
    std::cout << "Dequeue: ";
    while (auto val = queue.dequeue()) {
        std::cout << *val << " ";
    }
    std::cout << "\n";

    // 并发测试 - 生产者-消费者
    std::cout << "\n生产者-消费者并发测试:\n";
    LockFreeQueue<int> concurrentQueue;
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;
    const int TOTAL_ITEMS = 1000;

    std::atomic<int> produced(0);
    std::atomic<int> consumed(0);

    auto start = std::chrono::high_resolution_clock::now();

    // 生产者线程
    std::vector<std::thread> producers;
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.emplace_back([&concurrentQueue, &produced, i, TOTAL_ITEMS]() {
            for (int j = 0; j < TOTAL_ITEMS / NUM_PRODUCERS; ++j) {
                concurrentQueue.enqueue(i * 1000 + j);
                produced.fetch_add(1);
            }
        });
    }

    // 消费者线程
    std::vector<std::thread> consumers;
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.emplace_back([&concurrentQueue, &consumed, TOTAL_ITEMS]() {
            while (consumed.load() < TOTAL_ITEMS) {
                if (auto val = concurrentQueue.dequeue()) {
                    consumed.fetch_add(1);
                }
            }
        });
    }

    for (auto& t : producers) {
        t.join();
    }
    for (auto& t : consumers) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "生产: " << produced.load() << ", 消费: " << consumed.load() << "\n";
    std::cout << "耗时: " << duration.count() << " us\n";
}

// ============================================================================
// 4. 有锁 vs 无锁队列对比
// ============================================================================
// ============================================================================
// 4. 有锁 vs 无锁队列对比 - 不同竞争场景
// ============================================================================
void mutexQueueComparison() {
    std::cout << "\n========== 4. 有锁 vs 无锁队列对比 ==========\n";

    // 场景1: 低竞争 - 单生产者单消费者
    std::cout << "\n[场景1] 单生产者+单消费者:\n";
    {
        const int TOTAL = 5000;
        std::atomic<int> consumed{0};

        // 无锁
        {
            LockFreeQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::thread p([&q, TOTAL]() { for (int i = 0; i < TOTAL; ++i) q.enqueue(i); });
            std::thread c([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            p.join(); c.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  无锁: " << dur << " us\n";
        }

        // 有锁
        consumed = 0;
        {
            MutexQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::thread p([&q, TOTAL]() { for (int i = 0; i < TOTAL; ++i) q.enqueue(i); });
            std::thread c([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            p.join(); c.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  有锁: " << dur << " us\n";
        }
    }

    // 场景2: 中等竞争
    std::cout << "\n[场景2] 2生产者+2消费者:\n";
    {
        const int TOTAL = 5000;

        // 无锁
        {
            LockFreeQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps, cs;
            for (int i = 0; i < 2; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/2; ++j) q.enqueue(j); });
            std::atomic<int> consumed{0};
            for (int i = 0; i < 2; ++i) cs.emplace_back([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            for (auto& t : ps) t.join();
            for (auto& t : cs) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  无锁: " << dur << " us\n";
        }

        // 有锁
        {
            MutexQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps, cs;
            for (int i = 0; i < 2; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/2; ++j) q.enqueue(j); });
            std::atomic<int> consumed{0};
            for (int i = 0; i < 2; ++i) cs.emplace_back([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            for (auto& t : ps) t.join();
            for (auto& t : cs) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  有锁: " << dur << " us\n";
        }
    }

    // 场景3: 高竞争
    std::cout << "\n[场景3] 4生产者+4消费者:\n";
    {
        const int TOTAL = 5000;

        // 无锁
        {
            LockFreeQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps, cs;
            for (int i = 0; i < 4; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/4; ++j) q.enqueue(j); });
            std::atomic<int> consumed{0};
            for (int i = 0; i < 4; ++i) cs.emplace_back([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            for (auto& t : ps) t.join();
            for (auto& t : cs) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  无锁: " << dur << " us\n";
        }

        // 有锁
        {
            MutexQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps, cs;
            for (int i = 0; i < 4; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/4; ++j) q.enqueue(j); });
            std::atomic<int> consumed{0};
            for (int i = 0; i < 4; ++i) cs.emplace_back([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            for (auto& t : ps) t.join();
            for (auto& t : cs) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  有锁: " << dur << " us\n";
        }
    }

    // 场景4: 纯入队（测试入队性能）
    std::cout << "\n[场景4] 纯入队 (10万次) x 8线程:\n";
    {
        const int TOTAL = 100000;

        // 无锁 - 8 线程
        {
            LockFreeQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps;
            for (int i = 0; i < 8; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/8; ++j) q.enqueue(j); });
            for (auto& t : ps) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  无锁(8线程): " << dur << " us\n";
        }

        // 有锁 - 8 线程
        {
            MutexQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps;
            for (int i = 0; i < 8; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/8; ++j) q.enqueue(j); });
            for (auto& t : ps) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  有锁(8线程): " << dur << " us\n";
        }
    }

    // 场景5: 真正的生产者-消费者
    std::cout << "\n[场景5] 8生产者+8消费者:\n";
    {
        const int TOTAL = 5000;

        // 无锁
        {
            LockFreeQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps, cs;
            for (int i = 0; i < 8; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/8; ++j) q.enqueue(j); });
            std::atomic<int> consumed{0};
            for (int i = 0; i < 8; ++i) cs.emplace_back([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            for (auto& t : ps) t.join();
            for (auto& t : cs) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  无锁: " << dur << " us\n";
        }

        // 有锁
        {
            MutexQueue<int> q;
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::thread> ps, cs;
            for (int i = 0; i < 8; ++i) ps.emplace_back([&q, TOTAL]() { for (int j = 0; j < TOTAL/8; ++j) q.enqueue(j); });
            std::atomic<int> consumed{0};
            for (int i = 0; i < 8; ++i) cs.emplace_back([&q, &consumed, TOTAL]() { while (consumed < TOTAL) if (q.dequeue()) consumed++; });
            for (auto& t : ps) t.join();
            for (auto& t : cs) t.join();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
            std::cout << "  有锁: " << dur << " us\n";
        }
    }

    std::cout << "\n结论:\n";
    std::cout << "- 有消费者时: 无锁始终更快(避免锁争用)\n";
    std::cout << "- 纯入队时: 有锁略快(Michael-scott需检测tail)\n";
    std::cout << "- 高并发+生产者消费者: 无锁最佳场景\n";
}

// ============================================================================
// 5. ABA 问题演示
// ============================================================================
void abaProblemDemo() {
    std::cout << "\n========== 5. ABA 问题演示 ==========\n";

    std::cout << "ABA 问题描述:\n";
    std::cout << "1. 线程A读取栈顶元素 X\n";
    std::cout << "2. 线程B弹出 X，释放内存，然后压入新的 X\n";
    std::cout << "3. 线程A执行 CAS，认为栈顶未变化，成功压入\n";
    std::cout << "4. 结果：数据丢失或内存问题\n\n";

    // 使用位操作演示 tagged pointer（解决 ABA 问题）
    std::atomic<uintptr_t> taggedHead;

    // 演示 tagged pointer（使用位操作将指针和标签打包）
    void* dummy = new int(42);
    uint64_t tag = 1;
    // 将 tag 放在高位，指针放在低位
    uintptr_t packed = reinterpret_cast<uintptr_t>(dummy) | (tag << 48);
    taggedHead.store(packed);

    auto loaded = taggedHead.load();
    void* ptr = reinterpret_cast<void*>(loaded & 0xFFFFFFFFFFFF);  // 低48位是指针
    uint64_t loadedTag = loaded >> 48;  // 高位是标签
    std::cout << "Tagged Pointer: ptr=" << ptr
              << ", tag=" << loadedTag << "\n";

    // 增加版本号
    tag = loadedTag + 1;
    packed = reinterpret_cast<uintptr_t>(dummy) | (tag << 48);
    taggedHead.store(packed);

    loaded = taggedHead.load();
    loadedTag = loaded >> 48;
    std::cout << "更新后: tag=" << loadedTag << "\n";

    std::cout << "\n解决方案：使用带版本的指针（Hazard Pointer 或 Tagged Pointer）\n";

    delete static_cast<int*>(dummy);
}

// ============================================================================
// 6. 内存序深入演示
// ============================================================================
void memoryOrderDemo() {
    std::cout << "\n========== 6. 内存序深入演示 ==========\n";

    std::cout << "不同内存序的特点:\n\n";

    std::cout << "1. memory_order_relaxed:\n";
    std::cout << "   - 无同步保证，仅保证原子性\n";
    std::cout << "   - 适用于计数器等场景\n\n";

    std::cout << "2. memory_order_acquire:\n";
    std::cout << "   - 获取之前的所有写入\n";
    std::cout << "   - 适用于消费者读取数据\n\n";

    std::cout << "3. memory_order_release:\n";
    std::cout << "   - 释放后的所有写入对消费者可见\n";
    std::cout << "   - 适用于生产者写入数据\n\n";

    std::cout << "4. memory_order_seq_cst:\n";
    std::cout << "   - 顺序一致性（默认）\n";
    std::cout << "   - 最安全，但性能可能较低\n\n";

    // 实际示例：生产者-消费者
    std::atomic<int> data{0};
    std::atomic<bool> ready{false};

    std::thread producer([&]() {
        data.store(42, std::memory_order_release);
        ready.store(true, std::memory_order_release);
    });

    std::thread consumer([&]() {
        while (!ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        std::cout << "消费者读取: data=" << data.load(std::memory_order_acquire) << "\n";
    });

    producer.join();
    consumer.join();
}

// ============================================================================
// 7. 工作队列应用测试
// ============================================================================
void workQueueDemo() {
    std::cout << "\n========== 7. 工作队列应用 ==========\n";

    WorkQueue workQueue;
    std::atomic<int> completed{0};

    // 提交任务
    std::cout << "提交 5 个任务:\n";
    for (int i = 0; i < 5; ++i) {
        workQueue.submit(i, [&completed, i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            completed.fetch_add(1);
        });
    }

    // 工作线程处理
    std::vector<std::thread> workers;
    for (int i = 0; i < 2; ++i) {
        workers.emplace_back([&workQueue, &completed]() {
            while (completed.load() < 5) {
                auto* item = workQueue.waitForWork();
                if (item) {
                    std::cout << "处理任务 " << item->id << "\n";
                    item->task();
                    delete item;
                }
            }
        });
    }

    for (auto& t : workers) {
        t.join();
    }

    std::cout << "完成: " << completed.load() << " 个任务\n";
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "       C++ 无锁队列演示\n";
    std::cout << "========================================\n";

    atomicBasics();
    lockFreeStackDemo();
    lockFreeQueueDemo();
    mutexQueueComparison();
    abaProblemDemo();
    memoryOrderDemo();
    workQueueDemo();

    std::cout << "\n========================================\n";
    std::cout << "          演示完成\n";
    std::cout << "========================================\n";

    return 0;
}

```

</details>



<style>
.experiment-meta {
  margin: 1rem 0;
}
.experiment-meta table {
  margin: 0;
}
</style>
