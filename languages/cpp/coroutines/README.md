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
