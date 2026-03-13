# C++ Lambda 表达式 `C++14/17`

探索 C++ Lambda 表达式的强大功能。

> **语言版本**: C++11 或更高（推荐 C++14/17）

## 包含内容

1. Lambda 基础语法
2. 捕获列表（值捕获、引用捕获）
3. 泛型 Lambda (C++14)
4. Lambda 与 STL 算法结合
5. 立即调用的 Lambda (IIFE)
6. 可变参数 Lambda
7. Lambda 作为返回值
8. 函数式编程模式

## 基础语法

```cpp
[capture](parameters) -> return_type {
    // body
}
```

简化版本：
```cpp
auto add = [](int a, int b) { return a + b; };
auto result = add(1, 2);  // 3
```

## 捕获列表

| 捕获方式 | 语法 | 说明 |
|----------|------|------|
| 值捕获 | `[x]` | 复制当前作用域的变量 |
| 引用捕获 | `[&x]` | 引用当前作用域的变量 |
| 全部值捕获 | `[=]` | 复制所有变量 |
| 全部引用捕获 | `[&]` | 引用所有变量 |
| 混合捕获 | `[=, &x]` | 除了 x 引用，其他都复制 |
| 移动捕获 | `[x = std::move(x)]` | C++14 移动捕获 |

```cpp
int x = 10;
auto byValue = [x]() { return x; };     // 复制
auto byRef = [&x]() { return x; };      // 引用

x = 20;
std::cout << byValue() << "\n";  // 10 (仍然是复制时的值)
std::cout << byRef() << "\n";    // 20 (引用当前值)
```

## 泛型 Lambda (C++14)

```cpp
auto add = [](auto a, auto b) { return a + b; };
add(1, 2);      // int
add(1.5, 2.5); // double
add("a", "b"); // std::string
```

## STL 算法结合

```cpp
std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// 排序
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

// 过滤
std::copy_if(v.begin(), v.end(), output, [](int x) { return x > 3; });

// 转换
std::transform(v.begin(), v.end(), v.begin(), [](int x) { return x * 2; });

// 查找
auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });
```

## 立即调用函数表达式 (IIFE)

```cpp
// 初始化时进行计算
int x = []() {
    int sum = 0;
    for (int i = 0; i < 10; ++i) sum += i;
    return sum;
}();

// 作用域隔离变量
[](int x) {
    // 临时作用域
}(42);
```

## Lambda 作为返回值

```cpp
auto makeAdder(int n) {
    return [n](int x) { return x + n; };
}

auto add5 = makeAdder(5);
add5(3);  // 8
```

## std::function

将 Lambda 存储为函数对象：

```cpp
std::function<int(int, int)> func = [](int a, int b) { return a + b; };
```

## 捕获 this

```cpp
class Widget {
public:
    void process() {
        auto handler = [this]() {
            // 可以访问成员变量
            value_ = 42;
        };
        handler();
    }
private:
    int value_ = 0;
};
```

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/lambda/lambda
```

**手动编译：**
```bash
# Linux/macOS
g++ -std=c++20 -pthread -o lambda main.cpp
./lambda

# Windows (MSVC)
cl /std:c++20 /EHsc main.cpp
lambda.exe

# Windows (MinGW)
g++ -std=c++20 -o lambda.exe main.cpp
lambda.exe
```

## 最佳实践

1. **优先使用 auto** - Lambda 类型复杂，使用 auto 简化
2. **避免不必要的捕获** - 只捕获需要的变量
3. **引用捕获要小心** - 确保 Lambda 不会在捕获变量生命周期外使用
4. **使用 const 正确** - 不修改捕获变量时使用 const

## 要求

- C++11 或更高版本（推荐 C++17/20）
