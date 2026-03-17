# C++ 语言特性研究 (C++20/23)

本目录包含 C++ 语言特性的实验和研究，主要涵盖现代 C++ (C++20/23) 的核心特性。

> **语言版本要求**: C++20 或更高（条件允许时使用 C++23 特性）

## 快速开始 (CMake)

推荐使用 CMake 构建，跨平台统一：

```bash
# 进入 cpp 目录
cd languages/cpp

# 创建构建目录
cmake -B build

# 编译所有实验
cmake --build build

# 运行实验
./build/templates/templates          # Linux/macOS
.\build\templates\Debug\templates.exe  # Windows (MSVC)
./build/templates/templates.exe      # Windows (MinGW)
```

### 运行单个实验

```bash
# 模板实验
./build/templates/templates

# 智能指针实验
./build/smart-pointers/smart-pointers

# Lambda 实验
./build/lambda/lambda

# 移动语义实验
./build/move-semantics/move-semantics
```

### IDE 集成

CMake 支持主流 IDE：

**Visual Studio:**
```cmd
cmake -B build -G "Visual Studio 17 2022"
cmake --open build
```

**CLion / VS Code:**
直接打开 `languages/cpp` 目录，IDE 会自动识别 CMakeLists.txt。

---

## 实验列表

### 模板 (templates) `C++20`

探索 C++ 强大的模板系统。

**内容：**
- 函数模板和类模板
- 模板特化（完全特化、偏特化）
- 非类型模板参数
- 可变参数模板
- SFINAE（替换失败不是错误）
- Concepts (C++20)
- 编译时计算（模板元编程）

**运行：**
```bash
./build/templates/templates
```

---

### 智能指针 (smart-pointers) `C++11+`

探索 C++ 现代化的内存管理工具。

**内容：**
- `std::unique_ptr` - 独占所有权指针
- `std::shared_ptr` - 共享所有权指针
- `std::weak_ptr` - 弱引用指针
- 自定义删除器
- RAII（资源获取即初始化）模式
- 循环引用问题及解决方案
- 异常安全的资源管理

**运行：**
```bash
./build/smart-pointers/smart-pointers
```

---

### Lambda 表达式 (lambda) `C++14/17`

探索 C++ Lambda 表达式的功能和应用。

**内容：**
- Lambda 基础语法
- 捕获列表（值捕获、引用捕获）
- 泛型 Lambda (C++14)
- Lambda 与 STL 算法结合
- 立即调用的 Lambda (IIFE)
- 高阶函数和函数式编程
- 事件处理模式
- Lambda 在多线程中的应用

**运行：**
```bash
./build/lambda/lambda
```

---

### 移动语义 (move-semantics) `C++11+`

探索 C++11 引入的移动语义，提升程序性能。

**内容：**
- 左值 vs 右值
- 右值引用 (`T&&`)
- 移动构造函数和移动赋值运算符
- `std::move` 和移动语义
- 完美转发和 `std::forward`
- 返回值优化 (RVO/NRVO)
- 容器中的移动语义
- 性能对比和最佳实践

**运行：**
```bash
./build/move-semantics/move-semantics
```

---

### 并发编程 (concurrency) `C++11/17`

探索 C++ 多线程编程的核心概念和实用技术。

**内容：**
- `std::thread` - 线程创建和管理
- `std::mutex` - 互斥锁和锁的保护
- `std::lock_guard/std::unique_lock` - RAII 锁管理
- `std::condition_variable` - 条件变量和线程同步
- `std::atomic` - 原子操作和无锁编程
- `std::future/std::promise` - 异步任务和线程间通信
- `std::jthread` (C++20) - 自动 join 的线程
- 线程池实现
- 死锁避免

**运行：**
```bash
./build/concurrency/concurrency
```

---

### Ranges (ranges) `C++20`

探索 C++20 引入的 Ranges 库，实现组合式数据处理。

**内容：**
- `std::ranges::range` 概念
- `std::ranges::views` - 惰性视图
- 视图适配器 (filter, transform, take, drop 等)
- 管道操作符组合
- Ranges 算法

**运行：**
```bash
./build/ranges/ranges
```

---

### 协程 (coroutines) `C++20`

探索 C++20 引入的协程支持，实现异步编程和生成器模式。

**内容：**
- 协程基础 - 协程函数、协程句柄、promise_type
- `co_await` - 等待异步操作、自定义 awaitable
- `co_yield` - 产生值和生成器模式
- `co_return` - 从协程返回值
- `suspend_always`/`suspend_never` - 协程暂停策略
- Lazy vs Eager 协程 - 惰性执行 vs 立即执行
- Generator vs OptionalGenerator - 两种生成器设计模式
- 异常处理 - 协程中的异常传递

**关键概念：**
- 协程返回值由 `promise_type::get_return_object()` 创建
- `initial_suspend()` 决定协程是立即执行还是惰性执行
- 使用 RAII 包装协程句柄，避免内存泄漏

**运行：**
```bash
./build/coroutines/coroutines
```

---

## 手动编译 (不使用 CMake)

如果不想用 CMake，也可以手动编译：

**Linux/macOS:**
```bash
g++ -std=c++20 -o templates templates/main.cpp
./templates
```

**Windows (MSVC):**
```cmd
cl /std:c++20 /EHsc templates\main.cpp /Fe:templates.exe
templates.exe
```

**Windows (MinGW):**
```cmd
g++ -std=c++20 -o templates.exe templates/main.cpp
templates.exe
```

---

## 编译器要求

- **最低要求：** C++11（部分实验需要 C++14/17）
- **推荐：** C++20（支持 Concepts 等新特性）
- **支持的编译器：**
  - GCC 7.0+（推荐 10.0+）
  - Clang 5.0+（推荐 10.0+）
  - MSVC 2017+（推荐 2019+）

## 实验规范

每个实验目录包含：
- `CMakeLists.txt` - CMake 构建配置
- `meta.json` - 实验元数据
- `README.md` - 实验说明文档
- `main.cpp` - 可编译运行的实验代码

## 学习路径建议

1. **入门**：从 `lambda` 开始，学习现代 C++ 的函数式特性
2. **内存管理**：学习 `smart-pointers`，理解 RAII 和智能指针
3. **性能优化**：学习 `move-semantics`，掌握移动语义
4. **进阶**：学习 `templates`，深入理解 C++ 的泛型编程

## 相关链接

- [C++ 参考文档](https://zh.cppreference.com/)
- [现代 C++ 教程](https://github.com/changkun/modern-cpp-tutorial)
- [CLAUDE.md](../../CLAUDE.md) - AI 协作指南
