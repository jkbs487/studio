# C++ 协程 `C++20`

探索 C++20 协程编程的核心概念和实用技术。

> **语言版本**: C++20 或更高（协程是 C++20 引入的新特性）

## 包含内容

1. **协程基础** - 协程函数、协程句柄、promise_type
2. **co_await** - 等待异步操作
3. **co_yield** - 产生值和生成器模式
4. **co_return** - 从协程返回值
5. **suspend_always/suspend_never** - 协程暂停策略
6. **异步生成器** - 使用协程实现生成器
7. **Task 框架** - 轻量级异步任务
8. **协程与 future** - 协程与标准库异步编程的结合

## 核心概念

### 协程函数

C++20 中，添加了 `co_await`、`co_yield` 或 `co_return` 的函数即为协程函数：

```cpp
std::future<int> simpleCoroutine() {
    co_await std::suspend_always{};  // 暂停
    co_return 42;  // 返回值
}
```

### co_await 运算符

挂起当前协程，等待异步操作完成：

```cpp
auto result = co_await someAsyncOperation();
```

### co_yield 关键字

产生值并挂起协程（用于生成器）：

```cpp
generator<int> counter() {
    for (int i = 0; i < 10; ++i) {
        co_yield i;  // 产生值并暂停
    }
}
```

### promise_type

每个协程函数必须关联一个 promise_type，负责管理协程的状态：

```cpp
struct promise_type {
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    std::future<T> get_return_object() { return ...; }
    void return_value(T value) { ... }
    void unhandled_exception() { ... }
};
```

### 协程句柄

`std::coroutine_handle<>` 用于控制协程的执行：

```cpp
std::coroutine_handle<> handle = coroutineFunction();
handle.resume();  // 恢复执行
handle.destroy();  // 销毁协程
```

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
g++ -std=c++20 -fcoroutines -o coroutines main.cpp
./coroutines

# macOS (Apple Clang)
clang++ -std=c++20 -fcoroutines -o coroutines main.cpp
./coroutines
```

注意：部分编译器需要额外启用协程支持。

## 示例说明

### 生成器示例

```cpp
generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i) {
        co_yield i;  // 产生值
    }
}

// 使用
for (auto v : range(0, 5)) {
    std::cout << v << " ";  // 输出: 0 1 2 3 4
}
```

### 异步任务示例

```cpp
task<void> asyncWork() {
    co_await std::suspend_always{};  // 模拟异步等待
    std::cout << "Work completed\n";
}
```

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
   struct [[nodiscard]] task {
       struct promise_type {
           // ...
       };
   };
   ```

2. **正确处理异常**
   ```cpp
   void unhandled_exception() {
       std::terminate();
   }
   ```

3. **明确暂停策略**
   - `suspend_always`: 显式暂停，适合异步
   - `suspend_never`: 立即开始，适合同步

4. **避免内存泄漏**
   - 始终调用 `handle.destroy()` 或使用 RAII 包装器

## 常见错误

1. **忘记包含头文件** - 需要 `<coroutine>`
2. **未定义 promise_type** - 协程函数必须关联 promise_type
3. **协程句柄泄漏** - 忘记销毁协程导致内存泄漏
4. **在非协程函数中使用 co_xxx** - 编译错误
5. **暂停点选择不当** - 影响性能和正确性

## 要求

- C++20 兼容编译器
- 协程支持（部分编译器需要额外标志）

