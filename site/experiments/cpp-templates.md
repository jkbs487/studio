---
title: cpp-templates
description: 探索 C++ 模板系统：函数模板、类模板、模板特化、SFINAE等高级特性
---

# cpp-templates

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++20 |
| 创建时间 | 2026-03-12 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `template` `meta-programming` `generic-programming` |

</div>

## 描述

探索 C++ 模板系统：函数模板、类模板、模板特化、SFINAE等高级特性

# C++ 模板 `C++20`

探索 C++ 模板系统的强大功能。

> **语言版本**: C++20 或更高

## 包含内容

1. 函数模板
2. 类模板
3. 模板特化（完全特化和偏特化）
4. 非类型模板参数
5. 可变参数模板
6. SFINAE（替换失败不是错误）
7. Concepts (C++20)
8. 模板元编程示例

## 函数模板

```cpp
template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

max(1, 2);           // int
max(1.5, 2.5);       // double
max("a", "b");       // const char*
```

## 类模板

```cpp
template<typename T>
class Box {
public:
    T value;
    Box(T v) : value(v) {}
    T get() const { return value; }
};

Box<int> intBox(42);
Box<std::string> strBox("hello");
```

## 模板特化

### 完全特化

```cpp
template<>
class Box<const char*> {
public:
    const char* value;
    Box(const char* v) : value(v) {}
};
```

### 偏特化

```cpp
template<typename T>
class Box<T*> {
public:
    T* value;
    Box(T* v) : value(v) {}
    bool isNull() const { return value == nullptr; }
};

Box<int*> ptrBox(new int(42));
```

## 非类型模板参数

```cpp
template<int N>
class Array {
    int data[N];
};

Array<10> arr;  // 10个元素的数组

template<size_t N>
constexpr size_t stringLength(const char (&arr)[N]) {
    return N - 1;
}
```

## 可变参数模板

```cpp
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);
}

print(1, 2, 3, "hello");  // 输出: 1 2 3 hello

// 递归展开
void print() {}  // 基础情况

template<typename T, typename... Args>
void print(T value, Args... args) {
    std::cout << value << " ";
    print(args...);
}
```

## SFINAE

替换失败不是错误 - 编译器在模板推导时会跳过无效的替换。

```cpp
template<typename T>
std::enable_if_t<std::is_integral_v<T>> print(T value) {
    std::cout << "整数: " << value << "\n";
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>> print(T value) {
    std::cout << "浮点数: " << value << "\n";
}
```

## Concepts (C++20)

C++20 引入的概念提供更清晰的约束语法。

```cpp
template<typename T>
concept Addable = requires(T a, T b) {
    a + b;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}

// 或使用 requires 子句
template<typename T>
requires std::integral<T> || std::floating_point<T>
T add(T a, T b) {
    return a + b;
}
```

## 模板元编程

编译期计算。

### 阶乘

```cpp
template<int N>
constexpr int factorial = N * factorial<N - 1>;

template<>
constexpr int factorial<0> = 1;

static_assert(factorial<5> == 120);
```

### 类型列表

```cpp
template<typename... Args>
struct TypeList {};

template<typename List>
struct Size;

template<typename... Args>
struct Size<TypeList<Args...>> {
    static constexpr size_t value = sizeof...(Args);
};

static_assert(Size<TypeList<int, double, char>>::value == 3);
```

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/templates/templates
```

**手动编译：**
```bash
# Linux/macOS
g++ -std=c++20 -o templates main.cpp
./templates

# Windows (MSVC)
cl /std:c++20 /EHsc main.cpp
templates.exe

# Windows (MinGW)
g++ -std=c++20 -o templates.exe main.cpp
templates.exe
```

## 最佳实践

1. **使用 Concepts (C++20)** - 比 SFINAE 更易读
2. **避免模板代码膨胀** - 谨慎使用完全特化
3. **使用 auto** - 简化返回类型推断
4. **编译期计算** - 利用 constexpr 和模板元编程

## 要求

- C++20 标准或更高
- 支持 C++20 概念的编译器（gcc 10+, clang 10+, MSVC 2019+）


## 源码

<details>
<summary>main.cpp</summary>

```cpp
#include <iostream>
#include <string>
#include <type_traits>
#include <tuple>
#include <vector>
#include <array>

// ============================================
// 1. 函数模板
// ============================================

template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

// 多参数模板
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// ============================================
// 2. 类模板
// ============================================

template <typename T>
class Stack {
private:
    std::vector<T> elements;

public:
    void push(const T& value) {
        elements.push_back(value);
    }

    T pop() {
        if (elements.empty()) {
            throw std::out_of_range("Stack<>::pop(): empty stack");
        }
        T value = elements.back();
        elements.pop_back();
        return value;
    }

    bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }
};

// ============================================
// 3. 模板特化
// ============================================

// 通用模板
template <typename T>
class TypeInfo {
public:
    static void print() {
        std::cout << "未知类型" << std::endl;
    }
};

// 完全特化 - int
template <>
class TypeInfo<int> {
public:
    static void print() {
        std::cout << "类型: int (整数类型)" << std::endl;
    }
};

// 完全特化 - double
template <>
class TypeInfo<double> {
public:
    static void print() {
        std::cout << "类型: double (浮点类型)" << std::endl;
    }
};

// 偏特化 - 指针类型
template <typename T>
class TypeInfo<T*> {
public:
    static void print() {
        std::cout << "类型: " << typeid(T).name() << "* (指针类型)" << std::endl;
    }
};

// ============================================
// 4. 非类型模板参数
// ============================================

template <typename T, size_t Size>
class FixedArray {
private:
    std::array<T, Size> data;

public:
    FixedArray() : data{} {}

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const {
        return Size;
    }
};

// ============================================
// 5. 可变参数模板
// ============================================

// 编译时计算乘积
template <typename... Args>
auto multiply_all(Args... args) {
    return (args * ...);  // C++17 折叠表达式
}

// 打印可变参数
template <typename T>
void print_single(const T& value) {
    std::cout << value << " ";
}

template <typename First, typename... Rest>
void print_all(const First& first, const Rest&... rest) {
    std::cout << first << " ";
    if constexpr (sizeof...(rest) > 0) {
        print_all(rest...);
    }
}

// ============================================
// 6. SFINAE - 替换失败不是错误
// ============================================

// 只有当 T 是整数类型时才启用
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
only_for_integers(T value) {
    std::cout << "[整数] " << value << std::endl;
    return value * 2;
}

// 只有当 T 是浮点类型时才启用
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
only_for_integers(T value) {
    std::cout << "[浮点数] " << value << std::endl;
    return value / 2.0;
}

// C++20 使用 concepts 更简洁
template <typename T>
requires std::is_integral_v<T>
T square_integer(T value) {
    std::cout << "[概念-整数] 平方: ";
    return value * value;
}

// ============================================
// 7. 编译时计算（模板元编程）
// ============================================

// 编译时计算阶乘
template <unsigned int N>
struct Factorial {
    static constexpr unsigned int value = N * Factorial<N - 1>::value;
};

// 模板特化作为终止条件
template <>
struct Factorial<0> {
    static constexpr unsigned int value = 1;
};

// 编译时计算斐波那契数列
template <unsigned int N>
struct Fibonacci {
    static constexpr unsigned long long value =
        Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr unsigned long long value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr unsigned long long value = 1;
};

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ 模板实验" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 函数模板
    std::cout << "\n1. 函数模板:" << std::endl;
    std::cout << "max(10, 20) = " << max_value(10, 20) << std::endl;
    std::cout << "max(3.14, 2.71) = " << max_value(3.14, 2.71) << std::endl;
    std::cout << "add(10, 3.14) = " << add(10, 3.14) << std::endl;

    // 2. 类模板
    std::cout << "\n2. 类模板 (Stack<T>):" << std::endl;
    Stack<int> int_stack;
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    std::cout << "栈大小: " << int_stack.size() << std::endl;
    std::cout << "弹出的元素: " << int_stack.pop() << std::endl;
    std::cout << "栈大小: " << int_stack.size() << std::endl;

    Stack<std::string> string_stack;
    string_stack.push("Hello");
    string_stack.push("World");
    std::cout << "弹出的字符串: " << string_stack.pop() << std::endl;

    // 3. 模板特化
    std::cout << "\n3. 模板特化:" << std::endl;
    TypeInfo<int>::print();
    TypeInfo<double>::print();
    TypeInfo<std::string>::print();
    TypeInfo<int*>::print();

    // 4. 非类型模板参数
    std::cout << "\n4. 非类型模板参数:" << std::endl;
    FixedArray<int, 5> arr;
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = static_cast<int>(i * 10);
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }

    // 5. 可变参数模板
    std::cout << "\n5. 可变参数模板:" << std::endl;
    std::cout << "multiply_all(1, 2, 3, 4) = " << multiply_all(1, 2, 3, 4) << std::endl;
    std::cout << "multiply_all(2.5, 4) = " << multiply_all(2.5, 4.0) << std::endl;
    std::cout << "print_all: ";
    print_all("C++", "模板", "元编程", "很", "强大");
    std::cout << std::endl;

    // 6. SFINAE
    std::cout << "\n6. SFINAE 和类型约束:" << std::endl;
    only_for_integers(42);
    only_for_integers(3.14);
    std::cout << "square_integer(7) = " << square_integer(7) << std::endl;

    // 7. 模板元编程
    std::cout << "\n7. 编译时计算 (模板元编程):" << std::endl;
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Factorial<10>::value = " << Factorial<10>::value << std::endl;
    std::cout << "Fibonacci<10>::value = " << Fibonacci<10>::value << std::endl;
    std::cout << "Fibonacci<20>::value = " << Fibonacci<20>::value << std::endl;

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
