# C++ 并发编程

探索 C++ 多线程编程的核心概念和实用技术。

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
