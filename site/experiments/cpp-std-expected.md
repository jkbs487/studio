---
title: cpp-std-expected
description: 探索 C++23 std::expected 与 std::optional 对比：错误处理、链式操作、monadic 函数
---

# cpp-std-expected

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | cpp |
| 版本 | C++23 |
| 创建时间 | 2026-04-03 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `expected` `optional` `error-handling` `monadic` `c++23` `modern-cpp` |

</div>

## 描述

探索 C++23 std::expected 与 std::optional 对比：错误处理、链式操作、monadic 函数

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

## 源码

<details>
<summary>main.cpp</summary>

```cpp
#include <iostream>
#include <expected>    // C++23
#include <optional>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// ============================================================================
// 1. Basic std::expected vs std::optional
// ============================================================================
void basicComparison() {
    std::cout << "\n========== 1. Basic Comparison ==========\n";

    // std::optional - 有值或无值
    std::optional<int> opt1 = 42;
    std::optional<int> opt2 = std::nullopt;

    std::cout << "optional with value: " << opt1.value_or(-1) << "\n";
    std::cout << "optional nullopt: " << (opt2.has_value() ? "has value" : "no value") << "\n";

    // std::expected - 成功(T)或错误(E)
    std::expected<int, std::string> exp1 = 42;
    std::expected<int, std::string> exp2 = std::unexpected("Error occurred");

    std::cout << "expected with value: " << exp1.value() << "\n";
    std::cout << "expected with error: " << exp2.error() << "\n";
}

// ============================================================================
// 2. Creating expected
// ============================================================================
void creatingExpected() {
    std::cout << "\n========== 2. Creating expected ==========\n";

    // Implicit conversion from T
    std::expected<int, int> e1 = 10;
    std::cout << "Implicit: " << e1.value() << "\n";

    // Using std::unexpected
    std::expected<int, int> e2 = std::unexpected(-1);
    std::cout << "Unexpected: " << e2.error() << "\n";

    // Direct construction with std::unexpected
    std::expected<double, std::string> e3 = std::unexpected(std::string("invalid"));
    std::cout << "String error: " << (e3 ? "valid" : e3.error()) << "\n";

    // Factory function pattern
    auto makeSuccess = [](int v) -> std::expected<int, std::string> { return v; };
    auto makeError = [](std::string e) -> std::expected<int, std::string> { return std::unexpected(e); };

    std::cout << "Factory success: " << makeSuccess(100).value() << "\n";
    std::cout << "Factory error: " << makeError("failed").error() << "\n";
}

// ============================================================================
// 3. Error handling with expected
// ============================================================================
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}

std::expected<int, std::string> parseNumber(const std::string& s) {
    try {
        size_t pos;
        int result = std::stoi(s, &pos);
        if (pos != s.size()) {
            return std::unexpected("Invalid characters in number");
        }
        return result;
    } catch (...) {
        return std::unexpected("Cannot parse as integer");
    }
}

void errorHandling() {
    std::cout << "\n========== 3. Error Handling ==========\n";

    // Successful operations
    auto r1 = divide(10, 2);
    if (r1) {
        std::cout << "10/2 = " << *r1 << "\n";
    }

    auto r2 = parseNumber("42");
    if (r2) {
        std::cout << "Parsed: " << *r2 << "\n";
    }

    // Failed operations
    auto r3 = divide(10, 0);
    if (!r3) {
        std::cout << "Error: " << r3.error() << "\n";
    }

    auto r4 = parseNumber("not-a-number");
    if (!r4) {
        std::cout << "Parse error: " << r4.error() << "\n";
    }
}

// ============================================================================
// 4. std::expected with struct error
// ============================================================================
struct ParseError {
    int position;
    std::string message;

    ParseError(int pos, const std::string& msg) : position(pos), message(msg) {}
};

std::expected<int, ParseError> parseWithPosition(const std::string& s) {
    try {
        size_t pos;
        int result = std::stoi(s, &pos);
        if (pos != s.size()) {
            return std::unexpected(ParseError(static_cast<int>(pos), "Invalid character"));
        }
        return result;
    } catch (...) {
        return std::unexpected(ParseError(0, "Conversion failed"));
    }
}

void structuredErrors() {
    std::cout << "\n========== 4. Structured Errors ==========\n";

    auto result = parseWithPosition("123abc");
    if (!result) {
        std::cout << "Error at position " << result.error().position
                  << ": " << result.error().message << "\n";
    } else {
        std::cout << "Parsed: " << *result << "\n";
    }
}

// ============================================================================
// 5. Monadic operations (map, and_then) - C++26, not yet fully supported
// Note: These features are not available in current Clang/GCC
// ============================================================================
void monadicOperations() {
    std::cout << "\n========== 5. Monadic Operations (Manual) ==========\n";

    std::expected<int, std::string> e = 10;

    // Manual map - transform the value
    auto doubled = e ? std::expected<int, std::string>(*e * 2) : e;
    std::cout << "Manual map(*2): " << *doubled << "\n";

    // Manual map_error - transform the error
    auto transformedError = e ? e : std::unexpected("Error: " + e.error());
    std::cout << "Manual map_error: " << transformedError.error() << "\n";

    // Manual and_then - chain operations that return expected
    auto square = [](int x) -> std::expected<int, std::string> {
        if (x > 100) return std::unexpected("Too large");
        return x * x;
    };

    auto result1 = e ? square(*e) : e;
    std::cout << "Manual and_then(square): " << *result1 << "\n";

    // Chain multiple operations (manual)
    auto step1 = e ? std::expected<int, std::string>(*e + 1) : e;
    auto step2 = step1 ? square(*step1) : step1;
    auto chained = step2 ? std::expected<int, std::string>(*step2 - 1) : step2;
    std::cout << "Manual chained: " << *chained << "\n";

    // Error propagation
    std::expected<int, std::string> err = std::unexpected("original error");
    auto propagated = err ? err : err;  // error stays unchanged
    std::cout << "Error propagated: " << propagated.error() << "\n";
}

// ============================================================================
// 6. Comparison with optional for error handling
// ============================================================================
std::optional<int> findUserById_Optional(const std::vector<int>& users, int id) {
    auto it = std::find(users.begin(), users.end(), id);
    if (it != users.end()) {
        return std::distance(users.begin(), it);
    }
    return std::nullopt;
}

// Error types can explain WHY the operation failed
std::expected<int, std::string> findUserById_Expected(const std::vector<int>& users, int id) {
    if (id < 0) {
        return std::unexpected("Invalid ID: negative value");
    }
    auto it = std::find(users.begin(), users.end(), id);
    if (it != users.end()) {
        return static_cast<int>(std::distance(users.begin(), it));
    }
    return std::unexpected("User not found");
}

void optionalVsExpected() {
    std::cout << "\n========== 6. Optional vs Expected ==========\n";

    std::vector<int> users = {101, 102, 103};

    // Optional - just tells you if found
    auto opt = findUserById_Optional(users, 102);
    if (opt) {
        std::cout << "Optional: Found at index " << *opt << "\n";
    }

    auto optNotFound = findUserById_Optional(users, 999);
    if (!optNotFound) {
        std::cout << "Optional: Not found (no reason)\n";
    }

    // Expected - tells you if found AND why not found
    auto exp = findUserById_Expected(users, 102);
    if (exp) {
        std::cout << "Expected: Found at index " << *exp << "\n";
    }

    auto expNotFound = findUserById_Expected(users, 999);
    if (!expNotFound) {
        std::cout << "Expected: Not found - " << expNotFound.error() << "\n";
    }

    auto expInvalid = findUserById_Expected(users, -1);
    if (!expInvalid) {
        std::cout << "Expected: Invalid - " << expInvalid.error() << "\n";
    }
}

// ============================================================================
// 7. Real-world example: File parsing
// ============================================================================
struct FileError {
    enum Code { NotFound, PermissionDenied, InvalidFormat, Corrupted };
    Code code;
    std::string message;

    FileError(Code c, const std::string& msg) : code(c), message(msg) {}

    std::string toString() const {
        switch (code) {
            case NotFound: return "File not found";
            case PermissionDenied: return "Permission denied";
            case InvalidFormat: return "Invalid format";
            case Corrupted: return "File corrupted";
            default: return "Unknown error";
        }
    }
};

std::expected<std::vector<int>, FileError> readNumbers(const std::string& filename) {
    // Simulate file reading
    if (filename == "missing.txt") {
        return std::unexpected(FileError(FileError::NotFound, filename));
    }
    if (filename == "no-perm.txt") {
        return std::unexpected(FileError(FileError::PermissionDenied, filename));
    }
    if (filename == "bad-format.txt") {
        return std::unexpected(FileError(FileError::InvalidFormat, filename));
    }

    // Success case
    return std::vector<int>{1, 2, 3, 4, 5};
}

void realWorldExample() {
    std::cout << "\n========== 7. Real-world Example ==========\n";

    std::vector<std::string> files = {"numbers.txt", "missing.txt", "no-perm.txt", "bad-format.txt"};

    for (const auto& filename : files) {
        auto result = readNumbers(filename);
        if (result) {
            std::cout << filename << ": Read " << result->size() << " numbers: ";
            for (int n : *result) std::cout << n << " ";
            std::cout << "\n";
        } else {
            std::cout << filename << ": ERROR - " << result.error().toString() << "\n";
        }
    }
}

// ============================================================================
// 8. Value-or pattern with expected
// ============================================================================
void valueOrPattern() {
    std::cout << "\n========== 8. Value-or Pattern ==========\n";

    std::expected<int, std::string> success = 42;
    std::expected<int, std::string> failure = std::unexpected("Error");

    // Using value_or (like optional)
    std::cout << "Success value_or(-1): " << success.value_or(-1) << "\n";
    std::cout << "Failure value_or(-1): " << failure.value_or(-1) << "\n";

    // Manual fallback pattern (and_else is C++26)
    auto manualFallback = [](const std::expected<int, std::string>& exp)
        -> std::expected<int, std::string> {
        std::cout << "Fallback triggered: " << exp.error() << "\n";
        return 0;  // Default value
    };

    std::cout << "Success fallback: " << (success ? *success : 0) << "\n";
    std::cout << "Failure fallback: " << (failure ? *failure : 0) << "\n";
}

// ============================================================================
// 9. std::expected with pointers
// ============================================================================
struct Resource {
    int value;
    Resource(int v) : value(v) { std::cout << "Resource acquired: " << value << "\n"; }
    ~Resource() { std::cout << "Resource released: " << value << "\n"; }
};

std::expected<std::unique_ptr<Resource>, std::string> createResource(int value) {
    if (value < 0) {
        return std::unexpected("Negative value not allowed");
    }
    return std::make_unique<Resource>(value);
}

void expectedWithPointers() {
    std::cout << "\n========== 9. Expected with Pointers ==========\n";

    auto r1 = createResource(10);
    if (r1) {
        std::cout << "Created resource with value: " << (*r1)->value << "\n";
    }

    auto r2 = createResource(-5);
    if (!r2) {
        std::cout << "Failed: " << r2.error() << "\n";
    }
}

// ============================================================================
// 10. Converting between optional and expected
// ============================================================================
void convertingBetween() {
    std::cout << "\n========== 10. Converting Between Types ==========\n";

    // Optional to Expected: provide a default error
    std::optional<int> opt = 42;
    auto expFromOpt = opt ? std::expected<int, std::string>(*opt)
                          : std::unexpected<std::string>("No value");
    std::cout << "From optional: " << expFromOpt.value() << "\n";

    // Expected to Optional: just discard the error
    std::expected<int, std::string> exp = 100;
    auto optFromExp = exp ? std::optional<int>(*exp) : std::nullopt;
    std::cout << "To optional: " << (optFromExp ? std::to_string(*optFromExp) : "nullopt") << "\n";

    // Error to Optional
    std::expected<int, std::string> err = std::unexpected("error");
    auto optFromErr = err ? std::optional<int>(*err) : std::nullopt;
    std::cout << "Error to optional: " << (optFromErr ? "has value" : "nullopt") << "\n";
}

// ============================================================================
// 11. Template function with expected
// ============================================================================
template<typename T, typename E>
std::expected<T, E> safeDivide(T a, T b, E zeroError) {
    if (b == T{}) {
        return std::unexpected(zeroError);
    }
    return a / b;
}

void templateWithExpected() {
    std::cout << "\n========== 11. Template with Expected ==========\n";

    auto r1 = safeDivide<int, const char*>(10, 2, "");
    std::cout << "int divide: " << (r1 ? std::to_string(*r1) : r1.error()) << "\n";

    auto r2 = safeDivide<int, const char*>(10, 0, "Division by zero");
    std::cout << "int divide by zero: " << (r2 ? std::to_string(*r2) : r2.error()) << "\n";

    auto r3 = safeDivide<double, std::string>(3.14, 2.0, "Division by zero");
    std::cout << "double divide: " << (r3 ? std::to_string(*r3) : r3.error()) << "\n";
}

// ============================================================================
// Main
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "    C++23 std::expected vs std::optional\n";
    std::cout << "========================================\n";

    basicComparison();
    creatingExpected();
    errorHandling();
    structuredErrors();
    monadicOperations();
    optionalVsExpected();
    realWorldExample();
    valueOrPattern();
    expectedWithPointers();
    convertingBetween();
    templateWithExpected();

    std::cout << "\n========================================\n";
    std::cout << "          Demo Complete\n";
    std::cout << "========================================\n";

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
