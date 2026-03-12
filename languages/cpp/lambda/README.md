# C++ Lambda 表达式

探索 C++ Lambda 表达式的强大功能。

## 包含内容

1. Lambda 基础语法
2. 捕获列表（值捕获、引用捕获）
3. 泛型 Lambda (C++14)
4. Lambda 与 STL 算法结合
5. 立即调用的 Lambda
6. 可变参数 Lambda
7. Lambda 作为返回值
8. 函数式编程模式

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

## 要求

- C++11 或更高版本（推荐 C++17/20）
