# C++ 模板

探索 C++ 模板系统的强大功能。

## 包含内容

1. 函数模板
2. 类模板
3. 模板特化（完全特化和偏特化）
4. 非类型模板参数
5. 可变参数模板
6. SFINAE（替换失败不是错误）
7. 模板元编程示例

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

## 要求

- C++20 标准或更高
- 支持 C++20 概念的编译器（gcc 10+, clang 10+, MSVC 2019+）
