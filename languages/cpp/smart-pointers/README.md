# C++ 智能指针 `C++11+`

探索 C++ 现代化的内存管理工具。

> **语言版本**: C++11 或更高（推荐 C++17/20）

## 包含内容

1. `std::unique_ptr` - 独占所有权指针
2. `std::shared_ptr` - 共享所有权指针
3. `std::weak_ptr` - 弱引用指针
4. 自定义删除器
5. make_shared/make_unique 优化
6. 循环引用问题及解决方案
7. RAII (资源获取即初始化) 模式

## 核心概念

### std::unique_ptr

独占所有权的智能指针，作用域结束时自动释放内存。

```cpp
std::unique_ptr<int> p1 = std::make_unique<int>(42);
auto p2 = std::make_unique<MyClass>();

// 移动语义
std::unique_ptr<int> p3 = std::move(p1);  // p1 变为空

// 释放所有权
p2.release();  // 返回原始指针，unique_ptr 变为空
```

### std::shared_ptr

共享所有权的智能指针，使用引用计数。

```cpp
auto p1 = std::make_shared<int>(42);
std::shared_ptr<int> p2 = p1;  // 引用计数 +1

std::cout << p1.use_count() << "\n";  // 2
// p2 超出作用域，引用计数 -1
// p1 超出作用域，引用计数 0，释放内存
```

### std::weak_ptr

不参与引用计数的智能指针，用于打破循环引用。

```cpp
std::shared_ptr<Node> p1 = std::make_shared<Node>();
std::weak_ptr<Node> w = p1;

auto p2 = w.lock();  // 获取 shared_ptr
if (w.expired()) {}  // 检查是否过期
```

## 最佳实践

### 1. 使用 make_unique/make_shared

```cpp
// 推荐
auto p = std::make_unique<int>(42);

// 不推荐
std::unique_ptr<int> p(new int(42));
```

原因：避免内存泄漏、更好的异常安全、一次分配内存。

### 2. 自定义删除器

```cpp
auto p = std::unique_ptr<FILE, decltype(&fclose)>(
    fopen("file.txt", "r"), &fclose);

// 使用 lambda
auto p2 = std::unique_ptr<FILE, std::function<void(FILE*)>>(
    fopen("file.txt", "r"), [](FILE* f) { fclose(f); });
```

### 3. 避免循环引用

使用 `weak_ptr` 打破循环：

```cpp
struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 使用 weak_ptr
};
```

### 4. 传递给函数

```cpp
// 传递 unique_ptr - 使用引用或移动
void process(std::unique_ptr<int>& p);
void consume(std::unique_ptr<int> p);

// 传递 shared_ptr - 复制引用计数增加
void handle(std::shared_ptr<int> p);
void observe(const std::shared_ptr<int>& p);
```

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/smart-pointers/smart-pointers
```

**手动编译：**
```bash
# Linux/macOS
g++ -std=c++20 -o smart-pointers main.cpp
./smart-pointers

# Windows (MSVC)
cl /std:c++20 /EHsc main.cpp
smart-pointers.exe

# Windows (MinGW)
g++ -std=c++20 -o smart-pointers.exe main.cpp
smart-pointers.exe
```

## 常见错误

1. **循环引用** - 使用 shared_ptr 形成闭环，导致内存泄漏
2. **不必要的 shared_ptr** - 使用 shared_ptr 管理本地变量
3. **忘记释放** - 使用 new 而非 make_unique
4. **nullptr 解引用** - 使用前检查

## 对比

| 特性 | unique_ptr | shared_ptr | weak_ptr |
|------|------------|------------|----------|
| 所有权 | 独占 | 共享 | 无 |
| 引用计数 | 无 | 有 | 有 |
| 内存开销 | 1个指针 | 2个指针 | 1个指针 |
| 性能 | 最快 | 中等 | 最快 |

## 要求

- C++11 或更高版本（推荐 C++17/20）
