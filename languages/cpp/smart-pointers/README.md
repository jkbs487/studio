# C++ 智能指针

探索 C++ 现代化的内存管理工具。

## 包含内容

1. `std::unique_ptr` - 独占所有权指针
2. `std::shared_ptr` - 共享所有权指针
3. `std::weak_ptr` - 弱引用指针
4. 自定义删除器
5. make_shared/make_unique 优化
6. 循环引用问题及解决方案
7. RAII (资源获取即初始化) 模式

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

## 要求

- C++11 或更高版本（推荐 C++17/20）
