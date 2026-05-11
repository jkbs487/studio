---
title: cpp-ranges
description: 探索 C++20 Ranges 库：视图、管道操作符、组合式算法
---

# cpp-ranges

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++20 |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `ranges` `views` `c++20` `modern-cpp` |

</div>

## 描述

探索 C++20 Ranges 库：视图、管道操作符、组合式算法

# C++ Ranges `C++20`

探索 C++20 引入的 Ranges 库，实现组合式数据处理。

> **语言版本**: C++20 或更高

## 包含内容

1. **std::ranges::range 概念** - 理解 ranges 基础
2. **std::ranges::views** - 惰性视图（filter, transform, take, drop 等）
3. **视图适配器** - | 管道操作符组合
4. **std::ranges::algorithms** - 支持 ranges 的算法
5. **视图工厂** - iota, empty, single
6. **实际应用场景** - 数据处理管道
7. **概念和约束** - static_assert 验证类型

## 核心概念

### Range

任何可以迭代的对象（容器、数组、视图等）。

```cpp
std::vector<int> vec = {1, 2, 3};
static_assert(std::ranges::range<decltype(vec)>);  // true
```

### View（视图）

惰性求值的 Range，不拥有数据，只提供数据的"视图"。

```cpp
auto evens = vec | std::views::filter([](int x) { return x % 2 == 0; });
// 此时不进行计算，只有迭代时才计算
```

### 视图适配器 (View Adapter)

将一个 Range 转换为另一个 Range。

| 适配器 | 描述 |
|--------|------|
| `views::filter` | 过滤元素 |
| `views::transform` | 转换元素 |
| `views::take` | 取前 n 个 |
| `views::drop` | 跳过前 n 个 |
| `views::reverse` | 反转 |
| `views::split` | 分割 |
| `views::join` | 连接 |

### 视图工厂 (View Factory)

创建新的 Range。

| 工厂 | 描述 |
|------|------|
| `views::iota(n)` | 生成 n, n+1, n+2, ... |
| `views::iota(a, b)` | 生成 [a, b) |
| `views::single(x)` | 单元素视图 |
| `views::empty<T>` | 空视图 |

### 管道操作符

使用 `|` 组合视图适配器。

```cpp
auto result = vec
    | std::views::filter(...)
    | std::views::transform(...)
    | std::views::take(5);
```

## 实际应用示例

### 数据处理管道

```cpp
std::vector<double> prices = {100.0, 250.0, 50.0, 300.0};

auto discounted = prices
    | std::views::filter([](double p) { return p > 100; })
    | std::views::transform([](double p) { return p * 0.9; });
```

### 字符串处理

```cpp
std::string text = "hello world";
auto words = text | std::views::split(' ');
auto upper = words | std::views::transform([](auto w) {
    std::string s(w.begin(), w.end());
    for (char& c : s) c = std::toupper(c);
    return s;
});
```

### 组合操作

```cpp
std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int sum = 0;
int count = 0;
for (int x : nums | std::views::filter([](int n) { return n % 2 == 0; })) {
    if (count >= 3) break;
    sum += x * x;
    ++count;
}
```

## 惰性求值

Ranges 的核心特性：只有在迭代时才真正计算。

```cpp
auto pipeline = data
    | std::views::filter(...)
    | std::views::transform(...);

for (auto x : pipeline) { /* 第一次计算 */ }
for (auto x : pipeline) { /* 再次计算 */ }
```

每次迭代都会重新执行整个管道。

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/ranges/ranges
```

**手动编译：**
```bash
# Linux/macOS (需要 libstdc++ 支持 C++20)
g++ -std=c++20 -o ranges main.cpp
./ranges

# Clang (需要 libc++ 或 libstdc++ 支持)
clang++ -std=c++20 -stdlib=libc++ -o ranges main.cpp
./ranges
```

## 注意事项

### 命名空间

```cpp
using namespace std::ranges;  // 常用
// 或
std::ranges::views::filter  // 完整路径
```

### 避免歧义

在同时使用 std 和 ranges 的算法时，可能需要指定命名空间：

```cpp
std::sort(vec.begin(), vec.end());           // std 算法
std::ranges::sort(vec);                      // ranges 算法
```

### 视图不可复制

视图是只读的，不可复制但可移动。

### 概念检查

使用 static_assert 验证类型：

```cpp
static_assert(std::ranges::range<decltype(vec)>);
static_assert(std::ranges::view<decltype(myView)>);
static_assert(std::ranges::forward_range<decltype(vec)>);
```

## 局限性

- macOS 默认 clang 使用 libc++，Ranges 支持不完整
- 某些视图适配器需要 C++23
- 调试时视图链可能难以追踪

## 要求

- C++20 标准
- 支持 C++20 Ranges 的编译器 (GCC 10+, Clang 13+, MSVC 2019+)


## 源码

<details>
<summary>main.cpp</summary>

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>

// ============================================================================
// 辅助函数：打印视图（不复制）
// ============================================================================
void printVector(const std::string& name, const std::vector<int>& v) {
    std::cout << name << ": [";
    bool first = true;
    for (const auto& elem : v) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

void printVectorDouble(const std::string& name, const std::vector<double>& v) {
    std::cout << name << ": [";
    bool first = true;
    for (const auto& elem : v) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

// ============================================================================
// 1. 基础 range 和视图
// ============================================================================
void basicRanges() {
    std::cout << "\n========== 1. 基础 Range ==========\n";

    // 传统容器也是 range
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 使用 views 转换
    std::vector<int> doubled;
    for (int x : vec | std::views::transform([](int x) { return x * 2; })) {
        doubled.push_back(x);
    }
    printVector("原始向量", vec);
    printVector("2倍", doubled);

    // views::filter - 过滤元素
    std::vector<int> evens;
    for (int x : vec | std::views::filter([](int x) { return x % 2 == 0; })) {
        evens.push_back(x);
    }
    printVector("偶数", evens);

    // 管道操作符组合
    std::vector<int> result;
    for (int x : vec
        | std::views::filter([](int x) { return x > 2; })
        | std::views::transform([](int x) { return x * x; })) {
        result.push_back(x);
    }
    printVector("x > 2 -> x^2", result);
}

// ============================================================================
// 2. 视图工厂 (View Factories)
// ============================================================================
void viewFactories() {
    std::cout << "\n========== 2. 视图工厂 ==========\n";

    // views::iota - 生成整数序列
    std::vector<int> iotaVec;
    for (int x : std::views::iota(1, 10)) {  // [1, 2, ..., 9]
        iotaVec.push_back(x);
    }
    printVector("iota(1, 10)", iotaVec);

    // 无限序列 (使用 take 限制)
    std::vector<int> infinite;
    for (int x : std::views::iota(1) | std::views::take(10)) {
        infinite.push_back(x);
    }
    printVector("无限序列 take(10)", infinite);

    // views::single - 单元素视图
    auto singleView = std::views::single(42);
    std::vector<int> singleVec(singleView.begin(), singleView.end());
    printVector("views::single(42)", singleVec);
}

// ============================================================================
// 3. 视图适配器 (View Adapters)
// ============================================================================
void viewAdapters() {
    std::cout << "\n========== 3. 视图适配器 ==========\n";

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // views::take - 取前 n 个
    std::vector<int> take3;
    for (int x : vec | std::views::take(3)) take3.push_back(x);
    printVector("take(3)", take3);

    // views::drop - 跳过前 n 个
    std::vector<int> drop3;
    for (int x : vec | std::views::drop(3)) drop3.push_back(x);
    printVector("drop(3)", drop3);

    // views::reverse - 反转
    std::vector<int> reversed;
    for (int x : vec | std::views::reverse) reversed.push_back(x);
    printVector("reverse", reversed);

    // views::split - 分割
    std::string str = "hello,world";
    std::cout << "split(','): ";
    for (auto part : str | std::views::split(',')) {
        std::cout << std::string(part.begin(), part.end()) << " ";
    }
    std::cout << "\n";

    // views::join - 连接 (使用模拟方式)
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5}};
    std::vector<int> joined;
    for (const auto& inner : nested) {
        for (int x : inner) joined.push_back(x);
    }
    printVector("join (模拟)", joined);
}

// ============================================================================
// 4. Ranges 算法
// ============================================================================
void rangesAlgorithms() {
    std::cout << "\n========== 4. Ranges 算法 ==========\n";

    std::vector<int> vec = {5, 2, 8, 1, 9, 3};

    // 排序 - 使用 std::sort
    std::vector<int> sorted = vec;
    std::sort(sorted.begin(), sorted.end());
    printVector("sort", sorted);

    // 查找 - 使用 std::find
    auto it = std::find(vec.begin(), vec.end(), 8);
    if (it != vec.end()) {
        std::cout << "find(8): 找到 " << *it << "\n";
    }

    // 计数 - 使用 std::count_if
    vec = {1, 2, 2, 3, 2, 4};
    auto cnt = std::count_if(vec.begin(), vec.end(), [](int x) { return x == 2; });
    std::cout << "count(2): " << cnt << "\n";

    // 条件查找 - 使用 std::find_if
    auto it2 = std::find_if(vec.begin(), vec.end(), [](int x) { return x > 3; });
    if (it2 != vec.end()) {
        std::cout << "find_if(x > 3): " << *it2 << "\n";
    }

    // 范围检查 - 使用 std:: 版本
    std::cout << "all_of > 0: " << std::all_of(vec.begin(), vec.end(), [](int x) { return x > 0; }) << "\n";
    std::cout << "any_of > 5: " << std::any_of(vec.begin(), vec.end(), [](int x) { return x > 5; }) << "\n";

    // min/max
    std::cout << "min_element: " << *std::min_element(vec.begin(), vec.end()) << "\n";
    std::cout << "max_element: " << *std::max_element(vec.begin(), vec.end()) << "\n";

    // accumulate
    std::cout << "accumulate: " << std::accumulate(vec.begin(), vec.end(), 0) << "\n";
}

// ============================================================================
// 5. 投影 (Projections)
// ============================================================================
void projections() {
    std::cout << "\n========== 5. 投影 ==========\n";

    struct Person {
        std::string name;
        int age;
    };

    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    // 按 age 排序
    std::vector<Person> sortedPeople = people;
    std::sort(sortedPeople.begin(), sortedPeople.end(),
         [](const Person& a, const Person& b) { return a.age < b.age; });
    std::cout << "按 age 排序: ";
    for (const auto& p : sortedPeople) {
        std::cout << p.name << "(" << p.age << ") ";
    }
    std::cout << "\n";

    // 按 name 排序
    sortedPeople = people;
    std::sort(sortedPeople.begin(), sortedPeople.end(),
         [](const Person& a, const Person& b) { return a.name < b.name; });
    std::cout << "按 name 排序: ";
    for (const auto& p : sortedPeople) {
        std::cout << p.name << " ";
    }
    std::cout << "\n";

    // max with comparator
    Person oldest = *std::max_element(people.begin(), people.end(),
        [](const Person& a, const Person& b) { return a.age < b.age; });
    std::cout << "最年长: " << oldest.name << "\n";
}

// ============================================================================
// 6. 惰性求值示例
// ============================================================================
void lazyEvaluation() {
    std::cout << "\n========== 6. 惰性求值 ==========\n";

    std::vector<int> data = {1, 2, 3, 4, 5};

    // 创建视图链 - 不立即计算
    auto pipeline = data
        | std::views::filter([](int x) { return x % 2 == 1; })
        | std::views::transform([](int x) { return x * 2; })
        | std::views::take(2);

    std::cout << "创建视图链 (尚未计算)\n";

    // 迭代时才计算
    std::cout << "计算结果: ";
    for (int x : pipeline) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 对同一个视图多次迭代
    std::cout << "再次迭代: ";
    for (int x : pipeline) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

// ============================================================================
// 7. 实际应用场景
// ============================================================================
void realWorldScenarios() {
    std::cout << "\n========== 7. 实际应用场景 ==========\n";

    // 场景1: 数据处理管道
    std::cout << "场景1: 数据处理管道\n";
    std::vector<double> prices = {100.0, 250.0, 50.0, 300.0, 75.0};

    // 过滤 > 100, 打9折, 四舍五入, 排序
    std::vector<double> discounted;
    for (double p : prices
        | std::views::filter([](double x) { return x > 100; })
        | std::views::transform([](double x) { return std::round(x * 0.9); })) {
        discounted.push_back(p);
    }
    std::sort(discounted.begin(), discounted.end(), std::greater<double>());
    printVectorDouble("折扣后价格", discounted);

    // 场景2: 字符串处理
    std::cout << "\n场景2: 字符串处理\n";
    std::string text = "Hello World C++20 Ranges are awesome";

    // 找出所有单词，以大写形式输出
    std::vector<std::string> words;
    for (auto word : text | std::views::split(' ') | std::views::transform([](auto w) {
        std::string s(w.begin(), w.end());
        for (char& c : s) c = std::toupper(c);
        return s;
    })) {
        words.push_back(word);
    }

    std::cout << "单词 (大写): ";
    for (const auto& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n";

    // 场景3: 组合多个操作
    std::cout << "\n场景3: 组合操作\n";
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 找前3个偶数的平方和
    int sum = 0;
    int count = 0;
    for (int x : numbers | std::views::filter([](int x) { return x % 2 == 0; })) {
        if (count >= 3) break;
        sum += x * x;
        ++count;
    }
    std::cout << "前3个偶数的平方和: " << sum << "\n";
}

// ============================================================================
// 8. 概念和约束
// ============================================================================
void conceptsAndConstraints() {
    std::cout << "\n========== 8. 概念和约束 ==========\n";

    // 检查类型是否符合 range 概念
    std::vector<int> vec = {1, 2, 3};

    // range 概念
    static_assert(std::ranges::range<decltype(vec)>, "vector 是 range");
    static_assert(std::ranges::sized_range<decltype(vec)>, "vector 是 sized_range");

    // 检查视图
    auto view = vec | std::views::transform([](int x) { return x * 2; });
    static_assert(std::ranges::view<decltype(view)>, "transform view 是 view");
    static_assert(std::ranges::range<decltype(view)>, "transform view 是 range");

    // forward_range
    static_assert(std::ranges::forward_range<decltype(vec)>, "vector 是 forward_range");

    std::cout << "概念检查通过\n";
}

// ============================================================================
// 9. 自定义 Range (使用 iota)
// ============================================================================
void customRange() {
    std::cout << "\n========== 9. 自定义 Range ==========\n";

    // 使用 iota 替代 generate
    std::vector<int> custom;
    for (int n : std::views::iota(1, 6)
        | std::views::transform([](int n) { return n * n * n; })) {
        custom.push_back(n);
    }
    printVector("1^3 到 5^3", custom);

    // 模拟 repeat + take
    std::vector<int> repeated;
    for (int i = 0; i < 5; ++i) {
        repeated.push_back(7);
    }
    printVector("repeat(7) take(5)", repeated);
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "       C++20 Ranges 演示\n";
    std::cout << "========================================\n";

    basicRanges();
    viewFactories();
    viewAdapters();
    rangesAlgorithms();
    projections();
    lazyEvaluation();
    realWorldScenarios();
    conceptsAndConstraints();
    customRange();

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
