---
title: cpp-smart-pointers
description: 探索 C++ 智能指针：unique_ptr, shared_ptr, weak_ptr 以及 RAII 资源管理模式
---

# cpp-smart-pointers

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++11+ |
| 创建时间 | 2026-03-12 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `smart-pointer` `memory-management` `RAII` `modern-cpp` |

</div>

## 描述

探索 C++ 智能指针：unique_ptr, shared_ptr, weak_ptr 以及 RAII 资源管理模式

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


## 源码

<details>
<summary>main.cpp</summary>

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

// ============================================
// 示例类：用于演示智能指针
// ============================================

class Resource {
private:
    std::string name;
    int value;

public:
    explicit Resource(const std::string& n, int v = 0)
        : name(n), value(v) {
        std::cout << "[构造] " << name << " (value=" << value << ")" << std::endl;
    }

    ~Resource() {
        std::cout << "[析构] " << name << " (value=" << value << ")" << std::endl;
    }

    void doSomething() const {
        std::cout << "[操作] " << name << " 正在工作，值: " << value << std::endl;
    }

    void setValue(int v) {
        value = v;
    }

    int getValue() const {
        return value;
    }

    std::string getName() const {
        return name;
    }
};

// ============================================
// 1. std::unique_ptr - 独占所有权
// ============================================

void demo_unique_ptr() {
    std::cout << "\n--- 1. std::unique_ptr ---\n" << std::endl;

    // 创建 unique_ptr
    std::unique_ptr<Resource> res1 = std::make_unique<Resource>("Resource-1", 100);
    res1->doSomething();

    // 无法复制，只能移动
    std::unique_ptr<Resource> res2 = std::move(res1);
    std::cout << "移动后 res2: ";
    res2->doSomething();

    // res1 现在为空
    if (!res1) {
        std::cout << "res1 已移动，现在为空" << std::endl;
    }

    // 自定义删除器
    auto custom_deleter = [](Resource* p) {
        std::cout << "[自定义删除器] 正在删除 " << p->getName() << std::endl;
        delete p;
    };

    std::unique_ptr<Resource, decltype(custom_deleter)> res3(
        new Resource("Resource-3", 300), custom_deleter);

    // unique_ptr 与数组
    // 注意: make_unique<Resource[]>要求Resource有默认构造函数
    // 这里使用vector作为替代方案
    std::vector<std::unique_ptr<Resource>> arr;
    arr.push_back(std::make_unique<Resource>("Array-1", 101));
    arr.push_back(std::make_unique<Resource>("Array-2", 102));
    arr.push_back(std::make_unique<Resource>("Array-3", 103));
    std::cout << "创建动态数组 (使用vector<unique_ptr>): " << arr.size() << " 个元素" << std::endl;
}

// ============================================
// 2. std::shared_ptr - 共享所有权
// ============================================

void demo_shared_ptr() {
    std::cout << "\n--- 2. std::shared_ptr ---\n" << std::endl;

    // 创建 shared_ptr
    std::shared_ptr<Resource> res1 = std::make_shared<Resource>("Shared-1", 200);
    std::cout << "res1 引用计数: " << res1.use_count() << std::endl;

    // 复制 shared_ptr
    std::shared_ptr<Resource> res2 = res1;
    std::cout << "复制后 res1 引用计数: " << res1.use_count() << std::endl;
    std::cout << "res2 引用计数: " << res2.use_count() << std::endl;

    // 通过 shared_ptr 访问
    res1->doSomething();
    res2->doSomething();

    // 重置一个 shared_ptr
    res2.reset();
    std::cout << "res2.reset() 后 res1 引用计数: " << res1.use_count() << std::endl;

    // 在容器中使用 shared_ptr
    std::vector<std::shared_ptr<Resource>> vec;
    vec.push_back(res1);
    vec.push_back(std::make_shared<Resource>("Shared-2", 250));
    std::cout << "添加到 vector 后 res1 引用计数: " << res1.use_count() << std::endl;
}

// ============================================
// 3. std::weak_ptr - 弱引用
// ============================================

void demo_weak_ptr() {
    std::cout << "\n--- 3. std::weak_ptr ---\n" << std::endl;

    auto shared_res = std::make_shared<Resource>("Shared-3", 300);
    std::cout << "初始引用计数: " << shared_res.use_count() << std::endl;

    // 创建 weak_ptr，不增加引用计数
    std::weak_ptr<Resource> weak_res = shared_res;
    std::cout << "创建 weak_ptr 后引用计数: " << shared_res.use_count() << std::endl;

    // 检查 weak_ptr 是否有效
    if (auto locked = weak_res.lock()) {
        std::cout << "weak_ptr 有效，访问对象: ";
        locked->doSomething();
    }

    // 重置 shared_ptr
    shared_res.reset();
    std::cout << "shared_res.reset() 后引用计数: " << shared_res.use_count() << std::endl;

    // 再次尝试 lock
    if (auto locked = weak_res.lock()) {
        std::cout << "weak_ptr 仍有效" << std::endl;
    } else {
        std::cout << "weak_ptr 已失效（对象已被销毁）" << std::endl;
    }
}

// ============================================
// 4. 循环引用问题
// ============================================

class Node : public std::enable_shared_from_this<Node> {
private:
    std::string name;
    std::shared_ptr<Node> next;  // 使用 shared_ptr 会导致循环引用
    std::weak_ptr<Node> parent;   // 使用 weak_ptr 避免循环引用

public:
    explicit Node(const std::string& n) : name(n) {
        std::cout << "[Node构造] " << name << std::endl;
    }

    ~Node() {
        std::cout << "[Node析构] " << name << std::endl;
    }

    void setNext(const std::shared_ptr<Node>& n) {
        next = n;
        if (n) {
            n->parent = shared_from_this();  // 需要 enable_shared_from_this
        }
    }

    void display() const {
        std::cout << "Node: " << name;
        if (next) {
            std::cout << " -> " << next->name;
        }
        std::cout << std::endl;
    }

    std::string getName() const { return name; }
};

void demo_circular_reference() {
    std::cout << "\n--- 4. 循环引用演示 ---\n" << std::endl;

    // 使用 weak_ptr 可以避免循环引用
    auto node1 = std::make_shared<Node>("A");
    auto node2 = std::make_shared<Node>("B");

    node1->setNext(node2);
    node2->setNext(node1);

    std::cout << "\n链接关系:" << std::endl;
    node1->display();

    // 注意：如果没有使用 weak_ptr，这里会导致内存泄漏
    // 因为两个节点互相持有 shared_ptr，引用计数永远不会为0

    std::cout << "\nnode1 引用计数: " << node1.use_count() << std::endl;
    std::cout << "node2 引用计数: " << node2.use_count() << std::endl;

    // 由于使用了 weak_ptr，对象可以正常销毁
    std::cout << "\n离开作用域，观察析构顺序..." << std::endl;
}

// ============================================
// 5. 工厂函数最佳实践
// ============================================

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_custom(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 简单的对象池示例
class ObjectPool {
private:
    std::vector<std::unique_ptr<Resource>> pool;

public:
    void add(const std::string& name, int value) {
        pool.push_back(std::make_unique<Resource>(name, value));
    }

    Resource* get(size_t index) {
        if (index < pool.size()) {
            return pool[index].get();
        }
        return nullptr;
    }

    size_t size() const {
        return pool.size();
    }
};

void demo_best_practices() {
    std::cout << "\n--- 5. 最佳实践 ---\n" << std::endl;

    // 使用 make_shared 优化内存分配
    auto res = std::make_shared<Resource>("Optimized", 999);
    res->doSomething();

    // 在对象池中使用 unique_ptr
    ObjectPool pool;
    pool.add("Pool-1", 1);
    pool.add("Pool-2", 2);
    pool.add("Pool-3", 3);

    std::cout << "\n对象池大小: " << pool.size() << std::endl;
    if (auto obj = pool.get(1)) {
        obj->doSomething();
    }

    // 避免使用裸指针管理动态内存
    std::cout << "\n不推荐的写法（但展示对比）:" << std::endl;
    Resource* raw_ptr = new Resource("Raw-Ptr", 0);
    raw_ptr->doSomething();
    delete raw_ptr;  // 必须手动 delete，容易忘记
}

// ============================================
// 6. 异常安全示例
// ============================================

void riskyFunction(std::shared_ptr<Resource> res) {
    std::cout << "在函数中使用 shared_ptr" << std::endl;
    res->doSomething();
    // 即使抛出异常，资源也会被正确释放
    throw std::runtime_error("模拟异常");
}

void demo_exception_safety() {
    std::cout << "\n--- 6. 异常安全 ---\n" << std::endl;

    auto res = std::make_shared<Resource>("Exception-Safe", 500);

    try {
        riskyFunction(res);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }

    // res 仍然有效，资源未被泄露
    std::cout << "异常后资源仍然可用: ";
    res->doSomething();
}

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ 智能指针实验" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. unique_ptr
    demo_unique_ptr();

    // 2. shared_ptr
    demo_shared_ptr();

    // 3. weak_ptr
    demo_weak_ptr();

    // 4. 循环引用
    demo_circular_reference();

    // 5. 最佳实践
    demo_best_practices();

    // 6. 异常安全
    demo_exception_safety();

    std::cout << "\n========================================" << std::endl;
    std::cout << "实验完成!" << std::endl;
    std::cout << "========================================" << std::endl;

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
