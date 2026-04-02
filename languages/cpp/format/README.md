# std::format 格式化库 (C++20)

C++20 引入了 `<format>` 库，提供了类似 Python f-string 的类型安全格式化功能。

## 运行

```bash
# 使用 CMake
cd languages/cpp
cmake -B build
cmake --build build
./build/format/format

# 或直接编译
g++ -std=c++20 -o format format/main.cpp
./format
```

## 内容

- **基础格式化**: 位置参数、大括号转义
- **数字格式化**: 进制转换、精度、填充、对齐、千位分隔符
- **布尔值和指针**: 布尔格式化、指针十六进制输出
- **字符串格式化**: 截断、对齐、填充
- **日期时间格式化**: strftime 风格格式化
- **std::format_to**: 格式化到容器
- **动态宽度和精度**: 运行时指定格式
- **性能对比**: 与 printf、ostringstream 对比

## 特性

- **类型安全**: 编译时检查格式参数类型
- **可读性强**: 类似 Python format/pf-string 语法
- **本地化支持**: 可自定义格式化locale
- **惰性求值**: 格式化字符串按需处理
