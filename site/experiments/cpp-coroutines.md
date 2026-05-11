---
title: cpp-coroutines
description: 探索 C++20 协程：co_await、co_yield、co_return、协程句柄、promise_type
---

# cpp-coroutines

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++20 |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `coroutine` `co_await` `co_yield` `async` `modern-cpp` |

</div>

## 描述

探索 C++20 协程：co_await、co_yield、co_return、协程句柄、promise_type

# C++ 协程 `C++20`

探索 C++20 协程编程的核心概念和实用技术。

> **语言版本**: C++20 或更高（协程是 C++20 引入的新特性）

## 包含内容

1. **协程基础** - 协程函数、协程句柄、promise_type
2. **co_await** - 等待异步操作、自定义 awaitable
3. **co_yield** - 产生值和生成器模式
4. **co_return** - 从协程返回值
5. **suspend_always/suspend_never** - 协程暂停策略
6. **Lazy vs Eager 协程** - 惰性执行 vs 立即执行
7. **Generator vs OptionalGenerator** - 两种生成器设计模式
8. **异常处理** - 协程中的异常传递

## 核心概念

### 协程函数

C++20 中，添加了 `co_await`、`co_yield` 或 `co_return` 的函数即为协程函数：

```cpp
Task simpleCoroutine() {
    co_await std::suspend_always{};  // 暂停
    co_return;  // 返回
}
```

### 协程返回对象机制

协程的返回值由 `promise_type::get_return_object()` 创建，而不是函数体内的 `return`：

```cpp
// 编译器内部生成的伪代码
Task coroutineFunction() {
    auto& promise = /* 创建 promise */;
    Task result = promise.get_return_object();  // ← 创建返回值
    co_await promise.initial_suspend();
    // ... 协程体 ...
    return result;
}
```

**关键点**：
- 协程返回类型决定了查找哪个 `promise_type`
- `get_return_object()` 负责创建返回对象
- 协程体内不需要显式 `return`

### promise_type

每个协程函数必须关联一个 promise_type，负责管理协程的状态：

```cpp
struct promise_type {
    // 必需：创建返回对象
    Task get_return_object();
    
    // 必需：协程开始时的行为
    auto initial_suspend();  // suspend_always 或 suspend_never
    
    // 必需：协程结束时的行为
    auto final_suspend() noexcept;
    
    // 必需（二选一）：协程返回值
    void return_void();      // 用于 co_return; 或无返回语句
    void return_value(T);    // 用于 co_return value;
    
    // 必需：异常处理
    void unhandled_exception();
    
    // 可选：支持 co_yield
    auto yield_value(T);
};
```

### Lazy vs Eager 协程

| 类型 | `initial_suspend()` | 行为 |
|------|---------------------|------|
| **Lazy（惰性）** | `suspend_always` | 创建时不执行，需要手动 `resume()` |
| **Eager（急切）** | `suspend_never` | 创建时立即执行 |

```cpp
// Lazy 协程示例
struct LazyTask {
    struct promise_type {
        std::suspend_always initial_suspend() { return {}; }  // 先暂停
    };
    
    void resume() { handle.resume(); }  // 手动恢复
};

// Eager 协程示例
struct ImmediateTask {
    struct promise_type {
        std::suspend_never initial_suspend() { return {}; }  // 立即开始
    };
};
```

### co_await 运算符

挂起当前协程，等待异步操作完成：

```cpp
auto result = co_await someAsyncOperation();
```

**自定义 awaitable：**

```cpp
struct SyncSleepAwaitable {
    std::chrono::milliseconds duration;
    
    bool await_ready() const { return false; }
    
    void await_suspend(std::coroutine_handle<> h) const {
        std::this_thread::sleep_for(duration);
        h.resume();  // 手动恢复协程
    }
    
    void await_resume() const {}
};
```

**await_suspend 返回值行为：**
- 返回 `void`：协程暂停，需要手动 `resume()`
- 返回 `true`：协程暂停
- 返回 `false`：协程立即恢复

### co_yield 关键字

产生值并挂起协程（用于生成器）：

```cpp
Generator<int> counter() {
    for (int i = 0; i < 10; ++i) {
        co_yield i;  // 产生值并暂停
    }
}
```

### 协程句柄

`std::coroutine_handle<P>` 用于控制协程的执行：

```cpp
std::coroutine_handle<promise_type> handle;
handle.resume();   // 恢复执行
handle.destroy();   // 销毁协程
handle.promise();   // 访问 promise 对象
```

## Generator vs OptionalGenerator

两种生成器设计模式：

### Generator（传统风格）

```cpp
// next() 返回 bool，需要调用 current() 获取值
Generator<int> gen = countTo(5);
while (gen.next()) {
    std::cout << gen.current() << " ";
}
```

### OptionalGenerator（现代风格）

```cpp
// next() 直接返回 std::optional<T>
OptionalGenerator<int> gen = filterEven(10);
while (auto val = gen.next()) {
    std::cout << *val << " ";
}
```

| 特性 | Generator | OptionalGenerator |
|------|-----------|-------------------|
| `next()` 返回 | `bool` | `std::optional<T>` |
| 获取值 | `current()` | 解引用 `*val` |
| 调用次数 | 两步 | 一步 |
| 风格 | 传统迭代器 | 现代 functional |

## 异常处理

协程异常通过 `unhandled_exception()` 捕获，存储在 `std::exception_ptr` 中：

```cpp
struct promise_type {
    std::exception_ptr exception;
    
    void unhandled_exception() {
        exception = std::current_exception();
    }
};

// Generator 的 next() 方法需要重新抛出异常
bool next() {
    handle.resume();
    if (handle.done()) {
        if (handle.promise().exception) {
            std::rethrow_exception(handle.promise().exception);
        }
        return false;
    }
    return true;
}
```

**说明**：`std::exception_ptr` 和 `std::rethrow_exception` 是 C++11 特性。

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/coroutines/coroutines
```

**手动编译：**
```bash
# Linux/macOS
g++ -std=c++20 -o coroutines main.cpp
./coroutines

# macOS (Apple Clang)
clang++ -std=c++20 -fcoroutines -o coroutines main.cpp
./coroutines
```

注意：部分编译器需要额外启用协程支持。

## 编译器支持

| 编译器 | 版本要求 | 备注 |
|--------|----------|------|
| GCC | 10+ | 需要 -fcoroutines |
| Clang | 16+ | 实验性支持 |
| MSVC | 19.14+ | 默认支持 |
| Apple Clang | 14.0+ | 需要 -fcoroutines |

## 性能特点

- **轻量级**: 协程栈帧在堆上分配，开销远小于线程
- **高效切换**: 协程切换只需保存/恢复少量寄存器
- **适合 IO 密集**: 特别适合异步 I/O 操作
- **不适合 CPU 密集**: 多线程仍更适合 CPU 密集任务

## 最佳实践

1. **使用 RAII 管理生命周期**
   ```cpp
   struct Task {
       std::coroutine_handle<promise_type> handle;
       ~Task() { if (handle) handle.destroy(); }
       
       // 禁止复制
       Task(const Task&) = delete;
       Task& operator=(const Task&) = delete;
       
       // 允许移动
       Task(Task&& other) noexcept : handle(other.handle) {
           other.handle = nullptr;
       }
   };
   ```

2. **正确处理异常**
   ```cpp
   void unhandled_exception() {
       exception = std::current_exception();
   }
   
   // 在 next() 中重新抛出
   if (handle.promise().exception) {
       std::rethrow_exception(handle.promise().exception);
   }
   ```

3. **明确暂停策略**
   - `suspend_always`: 惰性执行，适合异步
   - `suspend_never`: 立即执行，适合同步

4. **避免内存泄漏**
   - 始终调用 `handle.destroy()` 或使用 RAII 包装器

## 常见错误

| 错误 | 原因 | 解决方案 |
|------|------|----------|
| 协程输出不完整 | `await_suspend` 返回 `void` 后未手动恢复 | 调用 `h.resume()` 恢复协程 |
| 异常未被捕获 | `next()` 未检查异常 | 使用 `std::rethrow_exception` |
| 内存泄漏 | 协程句柄未销毁 | 使用 RAII 包装器 |
| 编译错误：缺少成员 | `promise_type` 不完整 | 添加所有必需成员 |
| 类型未找到 | 返回类型没有 `promise_type` | 在返回类型中定义 `promise_type` |

## 要求

- C++20 兼容编译器
- 协程支持（部分编译器需要额外标志）

## 参考

- [C++ 协程参考](https://en.cppreference.com/w/cpp/language/coroutines)
- [C++20 协程教程](https://lewissbaker.github.io/)
- [FAQ.md](../../../FAQ.md) - 常见问题解答


## 源码

<details>
<summary>main.cpp</summary>

```cpp
#include <coroutine>
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <chrono>
#include <optional>
#include <stdexcept>

using namespace std::chrono_literals;

// ============================================================
// 基础协程概念
// ============================================================

// suspend_always: 总是暂停协程
struct suspend_always {
    constexpr bool await_ready() const noexcept { return false; }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

// suspend_never: 从不暂停协程（立即执行）
struct suspend_never {
    constexpr bool await_ready() const noexcept { return true; }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

// ============================================================
// 简单的 Task (协程返回对象)
// ============================================================

struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// ============================================================
// Generator (生成器) - 使用 co_yield
// ============================================================

template<typename T>
struct Generator {
    struct promise_type {
        T value;
        std::exception_ptr exception;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T val) {
            value = val;
            return {};
        }
        void return_void() {}
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> handle;

    Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    // Range-based for loop 支持
    bool next() {
        if (!handle) return false;
        if (handle.done()) return false;
        handle.resume();
        if (handle.done()) {
            // 协程已完成，检查是否有异常
            if (handle.promise().exception) {
                std::rethrow_exception(handle.promise().exception);
            }
            return false;
        }
        return true;
    }

    T& current() { return handle.promise().value; }
};

// ============================================================
// 演示1: 基础协程
// ============================================================

void basicCoroutine() {
    std::cout << "\n--- 基础协程 ---" << std::endl;

    // 简单的协程函数
    auto simpleCoroutine = []() -> Task {
        std::cout << "协程开始执行" << std::endl;
        co_await std::suspend_always{};  // 暂停
        std::cout << "协程恢复执行" << std::endl;
        co_return;
    };

    auto task = simpleCoroutine();
    std::cout << "协程已创建（暂停状态）" << std::endl;
    // Task 是立即暂停的，不需要手动恢复
    std::cout << "基础协程演示完成" << std::endl;
}

// ============================================================
// 演示2: 生成器 - co_yield
// ============================================================

Generator<int> countTo(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i;  // 产生值并暂停
    }
}

void generatorDemo() {
    std::cout << "\n--- 生成器 (co_yield) ---" << std::endl;

    // 创建生成器
    auto gen = countTo(5);

    std::cout << "生成 1-5:" << std::endl;
    while (gen.next()) {
        std::cout << gen.current() << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示3: 字符串生成器
// ============================================================

Generator<std::string> fibonacciStrings(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield "fib(" + std::to_string(i + 1) + ")=" + std::to_string(a);
        int next = a + b;
        a = b;
        b = next;
    }
}

void stringGeneratorDemo() {
    std::cout << "\n--- 字符串生成器 ---" << std::endl;

    auto gen = fibonacciStrings(8);
    while (gen.next()) {
        std::cout << gen.current() << ", ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示4: co_await 与自定义 awaitable
// ============================================================

// 同步等待的 awaitable
struct SyncSleepAwaitable {
    std::chrono::milliseconds duration;

    explicit SyncSleepAwaitable(std::chrono::milliseconds d) : duration(d) {}

    bool await_ready() const { return false; }

    void await_suspend(std::coroutine_handle<> h) const {
        std::this_thread::sleep_for(duration);
        h.resume();  // 手动恢复协程
    }

    void await_resume() const {}
};

// 立即执行的 Task (使用 suspend_never)
struct ImmediateTask {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        ImmediateTask get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return ImmediateTask{h};
        }
        std::suspend_never initial_suspend() { return {}; }  // 立即开始
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    ImmediateTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~ImmediateTask() { if (handle) handle.destroy(); }
    ImmediateTask(const ImmediateTask&) = delete;
    ImmediateTask& operator=(const ImmediateTask&) = delete;
    ImmediateTask(ImmediateTask&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    ImmediateTask& operator=(ImmediateTask&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }
};

ImmediateTask syncAsyncTask() {
    std::cout << "开始同步异步任务 (将等待 50ms)..." << std::endl;
    co_await SyncSleepAwaitable(50ms);
    std::cout << "同步异步任务完成!" << std::endl;
}

void coAwaitDemo() {
    std::cout << "\n--- co_await 自定义 awaitable ---" << std::endl;
    auto task = syncAsyncTask();
    // Task 会在这里析构，自动清理协程
}

// ============================================================
// 演示5: 协程返回值
// ============================================================

struct SimpleFuture {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        int value = 0;
        SimpleFuture get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return SimpleFuture{h};
        }
        std::suspend_never initial_suspend() { return {}; }  // 立即执行
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { value = v; }
        void unhandled_exception() { std::terminate(); }
    };

    int value() const { return handle.promise().value; }
};

SimpleFuture asyncAdd(int a, int b) {
    co_return a + b;
}

void futureCoroutineDemo() {
    std::cout << "\n--- 协程返回值 ---" << std::endl;

    auto fut = asyncAdd(10, 20);
    // 使用 suspend_never，协程立即执行，return_value 已被调用
    // 通过 handle 获取 promise 中的值
    std::cout << "10 + 20 = " << fut.value() << std::endl;
    fut.handle.destroy();  // 清理
}

// ============================================================
// 演示6: 无限生成器
// ============================================================

Generator<int> infiniteCounter() {
    int i = 0;
    while (true) {
        co_yield i;
        ++i;
    }
}

void infiniteGeneratorDemo() {
    std::cout << "\n--- 无限生成器 ---" << std::endl;

    auto counter = infiniteCounter();
    for (int i = 0; i < 10; ++i) {
        counter.next();
        std::cout << counter.current() << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示7: 使用 std::optional 的生成器
// ============================================================

template<typename T>
struct OptionalGenerator {
    struct promise_type {
        std::optional<T> value;
        std::exception_ptr exception;

        OptionalGenerator get_return_object() {
            return OptionalGenerator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T val) {
            value = val;
            return {};
        }
        void return_void() {}
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> handle;

    OptionalGenerator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~OptionalGenerator() { if (handle) handle.destroy(); }
    OptionalGenerator(OptionalGenerator&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    OptionalGenerator& operator=(OptionalGenerator&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    std::optional<T> next() {
        if (!handle || handle.done()) return std::nullopt;
        handle.resume();
        if (handle.done()) return std::nullopt;
        auto val = handle.promise().value;
        return val;
    }
};

OptionalGenerator<int> filterEven(int n) {
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            co_yield i;
        }
    }
}

void optionalGeneratorDemo() {
    std::cout << "\n--- 使用 std::optional 的生成器 ---" << std::endl;

    auto gen = filterEven(10);
    while (auto val = gen.next()) {
        std::cout << *val << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示8: 协程链 - 组合多个协程
// ============================================================

Generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i) {
        co_yield i;
    }
}

Generator<int> doubleValues(Generator<int> input) {
    while (input.next()) {
        co_yield input.current() * 2;
    }
}

void coroutineChainDemo() {
    std::cout << "\n--- 协程链 ---" << std::endl;

    std::cout << "range(1, 6): ";
    auto r = range(1, 6);
    while (r.next()) {
        std::cout << r.current() << " ";
    }
    std::cout << std::endl;

    std::cout << "doubleValues(range(1, 6)): ";
    auto doubled = doubleValues(range(1, 6));
    while (doubled.next()) {
        std::cout << doubled.current() << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示9: lazy 初始化协程（惰性执行）
// ============================================================

struct LazyTask {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        LazyTask get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return LazyTask{h};
        }
        std::suspend_always initial_suspend() { return {}; }  // 先暂停，不立即执行
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    LazyTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~LazyTask() { if (handle) handle.destroy(); }
    LazyTask(const LazyTask&) = delete;
    LazyTask& operator=(const LazyTask&) = delete;
    LazyTask(LazyTask&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    LazyTask& operator=(LazyTask&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    // 手动恢复执行
    void resume() {
        if (handle && !handle.done()) {
            handle.resume();
        }
    }

    bool done() const { return !handle || handle.done(); }
};

LazyTask lazyCoroutine() {
    std::cout << "lazy 协程: 开始执行" << std::endl;
    std::cout << "lazy 协程: 结束执行" << std::endl;
    co_return;
}

void lazyCoroutineDemo() {
    std::cout << "\n--- Lazy 初始化协程（惰性执行） ---" << std::endl;
    
    std::cout << "1. 创建协程任务..." << std::endl;
    auto task = lazyCoroutine();
    
    std::cout << "2. 协程已创建，但尚未执行（因为是 lazy）" << std::endl;
    
    std::cout << "3. 手动调用 resume()..." << std::endl;
    task.resume();
    
    std::cout << "4. 协程执行完毕" << std::endl;
}

// ============================================================
// 演示10: 异常处理
// ============================================================

Generator<int> exceptionGenerator(bool shouldThrow) {
    co_yield 1;
    co_yield 2;
    if (shouldThrow) {
        throw std::runtime_error("协程中的异常");
    }
    co_yield 3;
}

void exceptionDemo() {
    std::cout << "\n--- 协程异常处理 ---" << std::endl;

    // 不抛异常的生成器
    std::cout << "正常生成器: ";
    auto gen1 = exceptionGenerator(false);
    while (gen1.next()) {
        std::cout << gen1.current() << " ";
    }
    std::cout << std::endl;

    // 抛异常的生成器
    std::cout << "异常生成器: ";
    auto gen2 = exceptionGenerator(true);
    try {
        while (gen2.next()) {
            std::cout << gen2.current() << " ";
        }
    } catch (...) {
        std::cout << "捕获到异常!" << std::endl;
    }
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "C++20 协程实验" << std::endl;
    std::cout << "==============" << std::endl;

    basicCoroutine();
    generatorDemo();
    stringGeneratorDemo();
    coAwaitDemo();
    futureCoroutineDemo();
    infiniteGeneratorDemo();
    optionalGeneratorDemo();
    coroutineChainDemo();
    lazyCoroutineDemo();
    exceptionDemo();

    std::cout << "\n==============" << std::endl;
    std::cout << "实验完成!" << std::endl;
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
