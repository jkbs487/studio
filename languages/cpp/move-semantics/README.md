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

## 性能测试说明

### 修复历史

**2026-03-12: 性能测试修复**

**问题：**
- 原始性能测试存在 bug，导致移动语义比拷贝慢
- 测试方法不公平：拷贝测试只创建1次对象，移动测试创建10000次对象
- 性能测试产生大量输出，影响其他演示的可读性

**修复：**
1. 添加静默模式：`BigData` 类增加 `silent` 标志，性能测试时关闭构造/析构输出
2. 改进测试方法：改为测试真实场景 - vector 扩容时的拷贝 vs 预分配+移动
3. 测试配置：5000次循环，每个对象约39KB数据

**修复结果：**
```
拷贝 (vector扩容): 140 ms
移动 (预分配+move): 94 ms
性能提升: 1.48936x
时间节省: 46 ms
```

### 性能测试最佳实践

**原则：**
1. ✅ 确保测试条件公平 - 只对比要测量的部分
2. ✅ 避免隐藏的额外开销
3. ✅ 测试真实场景而非人为构造
4. ✅ 使用静默模式减少输出干扰
5. ✅ 多次运行取平均值更准确

**避免的错误：**
- ❌ 不同测试包含不同数量的构造函数调用
- ❌ 测试包含非目标操作的时间
- ❌ 循环次数太少导致测量不准确
- ❌ 数据量太小无法体现性能差异


