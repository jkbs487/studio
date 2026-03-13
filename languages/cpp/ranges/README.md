# C++ Ranges (C++20)

探索 C++20 引入的 Ranges 库，实现组合式数据处理。

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
