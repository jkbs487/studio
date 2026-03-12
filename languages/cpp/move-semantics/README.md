# C++ 移动语义

探索 C++11 引入的移动语义，提升程序性能。

## 包含内容

1. 左值 vs 右值
2. 右值引用 (`T&&`)
3. 移动构造函数
4. 移动赋值运算符
5. `std::move`
6. `std::forward` 和完美转发
7. 返回值优化 (RVO/NRVO)
8. 移动语义的性能优势

## 编译与运行

**使用 CMake（推荐）：**
```bash
# 在 cpp 目录下
cmake -B build
cmake --build build
./build/move-semantics/move-semantics
```

**手动编译：**
```bash
# Linux/macOS
g++ -std=c++20 -o move-semantics main.cpp
./move-semantics

# Windows (MSVC)
cl /std:c++20 /EHsc main.cpp
move-semantics.exe

# Windows (MinGW)
g++ -std=c++20 -o move-semantics.exe main.cpp
move-semantics.exe
```

## 要求

- C++11 或更高版本
