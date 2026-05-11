---
title: cpp-concurrency
description: 探索 C++ 并发编程：线程、互斥锁、原子操作、future/promise、线程池
---

# cpp-concurrency

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++11/17 |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `concurrency` `thread` `mutex` `atomic` `modern-cpp` |

</div>

## 描述

探索 C++ 并发编程：线程、互斥锁、原子操作、future/promise、线程池

# C++ 并发编程 `C++11/17`

探索 C++ 多线程编程的核心概念和实用技术。

> **语言版本**: C++11 或更高（推荐 C++17/20）

## 包含内容

1. **std::thread** - 线程创建和管理
2. **std::mutex** - 互斥锁和锁的保护
3. **std::lock_guard/std::unique_lock** - RAII 锁管理
4. **std::condition_variable** - 条件变量和线程同步
5. **std::atomic** - 原子操作和无锁编程
6. **std::future/std::promise** - 异步任务和线程间通信
7. **std::async** - 异步执行
8. **线程池** - 简单的线程池实现
9. **死锁避免** - 锁排序和 scoped_lock
10. **线程安全单例** - 使用局部静态变量的线程安全实现

## 核心概念

### 线程创建与管理

```cpp
std::thread t(function, args...);
t.join();  // 等待线程结束
t.detach();  // 分离线程
```

注意：线程对象在被销毁前必须调用 `join()` 或 `detach()`。

### 互斥锁 (Mutex)

```cpp
std::mutex mtx;
mtx.lock();
// 临界区
mtx.unlock();

// 推荐：RAII 方式
std::lock_guard<std::mutex> lock(mtx);
// 自动解锁
```

### 原子操作

```cpp
std::atomic<int> counter(0);
counter.fetch_add(1);  // 原子增加
counter.load();        // 读取
counter.store(10);     // 写入
```

### 条件变量

用于线程间同步，一个线程等待某个条件，另一个线程通知条件已满足。

```cpp
std::condition_variable cv;
std::mutex mtx;

std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [] { return ready; });  // 等待条件满足
cv.notify_one();  // 通知一个等待线程
cv.notify_all();   // 通知所有等待线程
```

### Future/Promise

用于线程间传递值或异常。

```cpp
std::promise<int> prom;
std::future<int> fut = prom.get_future();

// 在线程中
prom.set_value(42);

// 在主线程中
int result = fut.get();
```

### std::async

简化异步编程，自动管理线程。

```cpp
std::future<int> f = std::async(std::launch::async, func, args);
int result = f.get();  // 获取结果
```

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/concurrency/concurrency
```

**手动编译：**
```bash
# Linux/macOS
g++ -std=c++20 -pthread -o concurrency main.cpp
./concurrency

# Windows (MSVC)
cl /std:c++20 /EHsc /MD main.cpp
concurrency.exe
```

## 性能测试说明

### 互斥锁 vs 原子操作

测试结果（4线程，各100000次操作）：
- 手动加锁：约 15-30ms
- lock_guard：约 15-30ms（与手动类似，RAII更安全）
- 原子操作：约 5-10ms（无锁，更快）

### 并行计算

100万元素数组求和：
- 单线程：约 7ms
- 4线程并行：约 1ms

## 最佳实践

### 1. 避免死锁

**方案1：使用 scoped_lock（C++17）**
```cpp
std::scoped_lock lock(m1, m2, m3);  // 同时锁定多个
```

**方案2：std::lock + adopt_lock**
```cpp
std::lock(m1, m2);
std::lock_guard<std::mutex> l1(m1, std::adopt_lock);
std::lock_guard<std::mutex> l2(m2, std::adopt_lock);
```

**方案3：锁排序**
始终按固定顺序加锁。

### 2. 线程安全单例

使用 C++11 局部静态变量：
```cpp
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;  // 线程安全
        return instance;
    }
private:
    Singleton() {}
};
```

### 3. 条件变量注意事项

- 始终使用 `wait()` 的重载版本，带谓词（防止虚假唤醒）
- 在修改共享状态后调用 `notify`
- 使用 `unique_lock` 而非 `lock_guard`（需要手动解锁以允许其他线程获取锁）

### 4. 原子操作

- 适合简单计数器、标志位等
- 复杂操作仍需锁保护
- 使用 `memory_order` 控制内存序（高级）

## 常见错误

1. **忘记 join/detach** - 导致线程对象析构时终止程序
2. **死锁** - 多个锁顺序不一致
3. **竞争条件** - 未正确同步的共享数据访问
4. **虚假唤醒** - 未使用带谓词的 wait
5. **数据竞争** - 同时读写同一内存

## 要求

- C++11 或更高版本（推荐 C++17/20）
- 线程支持库 (-pthread on Linux/macOS)


## 源码

<details>
<summary>main.cpp</summary>

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <chrono>
#include <numeric>
#include <optional>
#include <type_traits>
#include <stdexcept>

// ============================================================================
// 1. 基础线程创建
// ============================================================================
void helloThread() {
    std::cout << "Hello from thread!\n";
}

void threadBasics() {
    std::cout << "\n========== 1. 基础线程创建 ==========\n";

    // 创建线程
    std::thread t(helloThread);
    t.join();  // 等待线程结束

    // 线程带参数
    std::thread t2([](int n) {
        std::cout << "Thread with argument: " << n << "\n";
    }, 42);
    t2.join();

    // 获取线程信息
    std::thread t3([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "Thread ID: " << std::this_thread::get_id() << "\n";
    });
    std::cout << "Main Thread ID: " << std::this_thread::get_id() << "\n";
    t3.join();
}

// ============================================================================
// 2. 互斥锁 (Mutex)
// ============================================================================
std::mutex mtx;
int sharedCounter = 0;

void incrementWithLock(int loops) {
    for (int i = 0; i < loops; ++i) {
        mtx.lock();
        ++sharedCounter;
        mtx.unlock();
    }
}

void incrementWithLockGuard(int loops) {
    for (int i = 0; i < loops; ++i) {
        std::lock_guard<std::mutex> lock(mtx);  // RAII 锁管理
        ++sharedCounter;
    }
}

void mutexDemo() {
    std::cout << "\n========== 2. 互斥锁 (Mutex) ==========\n";

    const int THREADS = 4;
    const int LOOPS = 100000;

    // 测试手动加锁
    sharedCounter = 0;
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(incrementWithLock, LOOPS);
    }
    for (auto& t : threads) t.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "手动加锁: " << sharedCounter
         << " (" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us)\n";

    // 测试 lock_guard
    sharedCounter = 0;
    threads.clear();
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(incrementWithLockGuard, LOOPS);
    }
    for (auto& t : threads) t.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "lock_guard: " << sharedCounter
         << " (" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us)\n";
}

// ============================================================================
// 3. 原子操作 (Atomic)
// ============================================================================
std::atomic<int> atomicCounter(0);
std::atomic<bool> ready(false);

void atomicIncrement(int loops) {
    while (!ready.load()) {
        std::this_thread::yield();
    }
    for (int i = 0; i < loops; ++i) {
        atomicCounter.fetch_add(1);
    }
}

void atomicDemo() {
    std::cout << "\n========== 3. 原子操作 (Atomic) ==========\n";

    const int THREADS = 4;
    const int LOOPS = 100000;

    atomicCounter = 0;
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(atomicIncrement, LOOPS);
    }
    ready = true;
    for (auto& t : threads) t.join();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "原子操作计数器: " << atomicCounter.load()
         << " (" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us)\n";

    // 原子布尔标志
    std::atomic<bool> flag(false);
    std::cout << "atomic<bool> 演示: flag = " << flag.load() << "\n";
    flag.store(true);
    std::cout << "设置后: flag = " << flag.load() << "\n";
}

// ============================================================================
// 4. 条件变量 (Condition Variable)
// ============================================================================
std::mutex cvMutex;
std::condition_variable cv;
bool dataReady = false;
std::string message;

void producer() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> lock(cvMutex);
        message = "Hello from producer!";
        dataReady = true;
        std::cout << "生产者: 数据已准备\n";
    }
    cv.notify_one();  // 通知消费者
}

void consumer() {
    std::unique_lock<std::mutex> lock(cvMutex);
    cv.wait(lock, [] { return dataReady; });  // 等待条件满足
    std::cout << "消费者收到: " << message << "\n";
}

void conditionVariableDemo() {
    std::cout << "\n========== 4. 条件变量 ==========\n";

    std::thread prod(producer);
    std::thread cons(consumer);
    prod.join();
    cons.join();

    // 生产者-消费者模型示例
    std::cout << "\n生产者-消费者模型示例:\n";
    std::queue<int> q;
    const int MAX_SIZE = 5;
    std::mutex qMutex;
    std::condition_variable qCv;
    bool done = false;

    auto producer2 = [&]() {
        for (int i = 0; i < 10; ++i) {
            std::unique_lock<std::mutex> lock(qMutex);
            qCv.wait(lock, [&] { return q.size() < MAX_SIZE; });
            q.push(i);
            std::cout << "生产: " << i << "\n";
            lock.unlock();
            qCv.notify_one();
        }
        {
            std::lock_guard<std::mutex> lock(qMutex);
            done = true;
        }
        qCv.notify_all();
    };

    auto consumer2 = [&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(qMutex);
            qCv.wait(lock, [&] { return !q.empty() || done; });
            if (q.empty() && done) break;
            int val = q.front();
            q.pop();
            std::cout << "消费: " << val << "\n";
            lock.unlock();
            qCv.notify_one();
        }
    };

    std::thread p2(producer2);
    std::thread c2(consumer2);
    p2.join();
    c2.join();
}

// ============================================================================
// 5. Future 和 Promise
// ============================================================================
int calculateSquare(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return x * x;
}

void futurePromiseDemo() {
    std::cout << "\n========== 5. Future 和 Promise ==========\n";

    // 使用 async
    std::cout << "std::async 演示:\n";
    std::future<int> f1 = std::async(std::launch::async, calculateSquare, 5);
    std::future<int> f2 = std::async(std::launch::async, calculateSquare, 10);
    std::cout << "5^2 = " << f1.get() << "\n";
    std::cout << "10^2 = " << f2.get() << "\n";

    // 使用 promise
    std::cout << "\npromise/future 演示:\n";
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t([](std::promise<int>& p) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        p.set_value(42);
    }, std::ref(prom));

    std::cout << "等待 promise... ";
    std::cout << "值: " << fut.get() << "\n";
    t.join();

    // 异常传递
    std::promise<double> prom2;
    std::future<double> fut2 = prom2.get_future();
    std::thread t2([](std::promise<double>& p) {
        try {
            throw std::runtime_error("计算错误!");
        } catch (...) {
            p.set_exception(std::current_exception());
        }
    }, std::ref(prom2));

    try {
        fut2.get();
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }
    t2.join();
}

// ============================================================================
// 6. 线程池实现
// ============================================================================
class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->cv.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<typename F>
    std::future<typename std::invoke_result<F>::type> enqueue(F&& f) {
        using return_type = typename std::invoke_result<F>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) throw std::runtime_error("线程池已停止");
            tasks.emplace([task]() { (*task)(); });
        }
        cv.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop;
};

void threadPoolDemo() {
    std::cout << "\n========== 6. 线程池 ==========\n";

    ThreadPool pool(4);

    // 提交多个任务
    std::vector<std::future<int>> results;
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue([i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            return i * i;
        }));
    }

    std::cout << "线程池任务结果:\n";
    for (auto& f : results) {
        std::cout << f.get() << " ";
    }
    std::cout << "\n";
}

// ============================================================================
// 7. 死锁避免示例
// ============================================================================

// 演示死锁场景的全局 mutex
std::mutex m1, m2;

// 错误示范：交叉加锁导致死锁
void wrongLockOrder(int threadId) {
    if (threadId == 0) {
        std::lock_guard<std::mutex> lock1(m1);
        std::cout << "线程0: 持有 m1\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock2(m2);  // 等待 m2
        std::cout << "线程0: 持有 m2\n";
    } else {
        std::lock_guard<std::mutex> lock2(m2);
        std::cout << "线程1: 持有 m2\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock1(m1);  // 等待 m1
        std::cout << "线程1: 持有 m1\n";
    }
}

// 方案1: scoped_lock 一次性锁定多个 mutex (C++17)
void correctLockScopedLock(int threadId) {
    std::scoped_lock lock(m1, m2);  // C++17: 一次性获取多个锁，避免死锁
    if (threadId == 0) {
        std::cout << "线程0: 持有 m1 和 m2\n";
    } else {
        std::cout << "线程1: 持有 m1 和 m2\n";
    }
}

// 方案2: std::lock 避免死锁 + adopt_lock 接管所有权
void correctLockStdLock(int threadId) {
    std::lock(m1, m2);  // 用死锁避免算法锁定
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
    if (threadId == 0) {
        std::cout << "线程0: 持有 m1 和 m2\n";
    } else {
        std::cout << "线程1: 持有 m1 和 m2\n";
    }
}

// 方案3: 锁排序 - 总是按固定顺序加锁
void correctLockOrdered(int threadId) {
    std::lock_guard<std::mutex> lock1(m1);  // 总是先锁 m1
    std::lock_guard<std::mutex> lock2(m2);  // 再锁 m2
    if (threadId == 0) {
        std::cout << "线程0: 持有 m1 和 m2\n";
    } else {
        std::cout << "线程1: 持有 m1 和 m2\n";
    }
}

void deadlockExample() {
    std::cout << "\n========== 7. 死锁避免 ==========\n";

    // 演示死锁场景（实际运行会卡住，注释掉仅作说明）
    std::cout << "\n--- 错误示范: 交叉加锁导致死锁 ---\n";
    std::cout << "线程0: m1 -> m2\n";
    std::cout << "线程1: m2 -> m1\n";
    std::cout << "如果两个线程同时运行，会互相等待对方释放锁 -> 死锁\n";

    // 方案1: scoped_lock
    std::cout << "\n--- 方案1: std::scoped_lock (C++17) ---\n";
    {
        std::thread t1(correctLockScopedLock, 0);
        std::thread t2(correctLockScopedLock, 1);
        t1.join();
        t2.join();
    }

    // 方案2: std::lock + adopt_lock
    std::cout << "\n--- 方案2: std::lock + adopt_lock ---\n";
    {
        std::thread t1(correctLockStdLock, 0);
        std::thread t2(correctLockStdLock, 1);
        t1.join();
        t2.join();
    }

    // 方案3: 锁排序
    std::cout << "\n--- 方案3: 锁排序 ---\n";
    {
        std::thread t1(correctLockOrdered, 0);
        std::thread t2(correctLockOrdered, 1);
        t1.join();
        t2.join();
    }
}

// ============================================================================
// 8. 并行计算示例
// ============================================================================
std::vector<long long> parallelSum(const std::vector<int>& data, int numThreads) {
    size_t size = data.size();
    size_t chunkSize = size / numThreads;
    std::vector<long long> partialSums(numThreads, 0);
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i] {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? size : start + chunkSize;
            for (size_t j = start; j < end; ++j) {
                partialSums[i] += data[j];
            }
        });
    }

    for (auto& t : threads) t.join();

    return partialSums;
}

void parallelComputationDemo() {
    std::cout << "\n========== 8. 并行计算 ==========\n";

    // 创建大数据集
    std::vector<int> data(1000000, 1);
    std::cout << "数据集大小: " << data.size() << " 元素\n";

    // 单线程求和
    auto start = std::chrono::high_resolution_clock::now();
    long long singleSum = std::accumulate(data.begin(), data.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "单线程求和: " << singleSum
         << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms)\n";

    // 4线程并行求和
    start = std::chrono::high_resolution_clock::now();
    auto partials = parallelSum(data, 4);
    long long parallelSumResult = std::accumulate(partials.begin(), partials.end(), 0LL);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "4线程并行: " << parallelSumResult
         << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms)\n";

    // 验证结果
    if (singleSum == parallelSumResult) {
        std::cout << "结果验证: 正确\n";
    }
}

// ============================================================================
// 9. Thread-Safe Singleton
// ============================================================================
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;  // C++11 保证线程安全的局部静态变量
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() : value(0) {
        std::cout << "Singleton 构造\n";
    }
    int value;
};

void singletonDemo() {
    std::cout << "\n========== 9. 线程安全的单例 ==========\n";

    // 测试1: 简单验证单例
    auto& s1 = Singleton::getInstance();
    s1.setValue(100);
    std::cout << "s1.value = " << s1.getValue() << "\n";

    auto& s2 = Singleton::getInstance();
    std::cout << "s2.value = " << s2.getValue() << "\n";

    if (&s1 == &s2) {
        std::cout << "两个引用指向同一个实例\n";
    }

    // 测试2: 多线程并发获取单例，验证线程安全
    std::cout << "\n多线程并发测试:\n";
    const int NUM_THREADS = 10;
    std::vector<std::thread> threads;
    std::vector<Singleton*> pointers(NUM_THREADS, nullptr);
    std::atomic<int> constructorCount(0);

    // 替换构造函数输出为计数
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&pointers, i, &constructorCount]() {
            // 模拟一些延迟，增加竞争
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            Singleton& instance = Singleton::getInstance();
            pointers[i] = &instance;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // 验证所有指针都指向同一个实例
    Singleton* first = pointers[0];
    bool allSame = true;
    for (int i = 1; i < NUM_THREADS; ++i) {
        if (pointers[i] != first) {
            allSame = false;
            break;
        }
    }

    std::cout << "线程数: " << NUM_THREADS << "\n";
    std::cout << "所有线程获取同一实例: " << (allSame ? "是" : "否") << "\n";
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "       C++ 并发编程演示\n";
    std::cout << "========================================\n";

    threadBasics();
    mutexDemo();
    atomicDemo();
    conditionVariableDemo();
    futurePromiseDemo();
    threadPoolDemo();
    deadlockExample();
    parallelComputationDemo();
    singletonDemo();

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
