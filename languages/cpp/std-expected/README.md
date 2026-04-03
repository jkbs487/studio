# std::expected (C++23)

C++23 引入了 `<expected>` 库，提供了类型安全的错误处理方式，类似于 `std::optional` 但可以携带错误信息。

## 运行

```bash
# 使用 CMake
cd languages/cpp
cmake -B build
cmake --build build
./build/std-expected/std-expected

# 或直接编译 (需要 C++23 支持)
g++ -std=c++23 -o std-expected std-expected/main.cpp
./std-expected
```

## 内容

- **基础对比**: `std::expected` vs `std::optional` 的区别
- **创建方式**: 隐式构造、`std::unexpected`、工厂函数
- **错误处理**: 检查成功/失败、获取值或错误
- **结构化错误**: 自定义错误类型携带更多上下文
- **monadic 操作**: `map`、`map_error`、`and_then` 链式调用
- **optional vs expected**: 何时使用哪个
- **实际示例**: 文件解析、错误传播
- **value-or 模式**: `value_or` 和 `and_else`
- **与指针结合**: `expected<unique_ptr<T>, E>`
- **类型转换**: optional 与 expected 互转
- **模板函数**: 泛型错误处理

## 特性

- **类型安全**: 编译时检查，避免运行时错误码
- **语义明确**: 区分"无值"和"错误"两种情况
- **链式操作**: 支持函数式风格的错误传播
- **错误信息**: 可携带丰富的错误上下文

## 使用场景

### 1. 函数返回错误

```cpp
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) return std::unexpected("Division by zero");
    return a / b;
}

auto result = divide(10, 2);
if (result) {
    std::cout << *result << "\n";
} else {
    std::cerr << result.error() << "\n";
}
```

### 2. 链式操作

```cpp
std::expected<int, std::string> parse(const std::string& s);

auto result = parse("42")
    .map([](int x) { return x * 2; })        // 转换值
    .map_error([](std::string e) {           // 转换错误
        return "Parse error: " + e;
    });
```

### 3. 结构化错误

```cpp
struct FileError {
    enum Code { NotFound, PermissionDenied, InvalidFormat };
    Code code;
    std::string message;
};

std::expected<Data, FileError> readFile(const std::string& path);
```

## optional vs expected

| 特性 | std::optional | std::expected |
|------|---------------|---------------|
| 语义 | 有值或无值 | 成功(T)或失败(E) |
| 错误信息 | 无 | 可携带详细错误 |
| 使用场景 | 可选参数、可能不存在的值 | 需要报告失败原因 |
| 链式操作 | `and_then`, `transform` | `and_then`, `map`, `map_error` |

## 注意事项

- 编译器需要支持 C++23（GCC 13+、Clang 16+、MSVC 2022 17.6+）
- 错误类型必须是可复制的（Copyable）
- `std::unexpected` 用于构造错误值
- `*` 和 `.value()` 获取值，`.error()` 获取错误