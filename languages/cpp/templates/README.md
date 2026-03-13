# C++ 模板

探索 C++ 模板系统的强大功能。

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
