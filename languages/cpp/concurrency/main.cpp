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
std::mutex m1, m2;

void deadlockExample() {
    std::cout << "\n========== 7. 死锁避免 ==========\n";

    // 方案1: scoped_lock 一次性锁定多个 mutex (C++17)
    {
        std::scoped_lock lock(m1, m2);  // C++17 scoped_lock，自动解锁多个
        std::cout << "使用 scoped_lock 锁定多个 mutex\n";
    }

    // 方案2: std::lock 避免死锁 + adopt_lock 接管所有权
    {
        // std::lock 会使用避免死锁的算法锁定所有 mutex
        std::lock(m1, m2);
        std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
        std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
        std::cout << "使用 std::lock + adopt_lock\n";
    }

    // 方案3: 锁排序 - 总是按固定顺序加锁
    std::cout << "锁排序避免死锁\n";
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
