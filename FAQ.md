# 常见问题 (FAQ)

本文档收集了在使用 Studio 代码库时常见的问题和解决方案。

## 目录

- [C++ 相关](#c-相关)
- [编译和构建](#编译和构建)
- [代码问题](#代码问题)

---

## C++ 相关

### 编译器未找到

**错误信息：**
```
cmake ..
-- The CXX compiler identification is unknown
CMake Error at CMakeLists.txt:2 (project):
  No CMAKE_CXX_COMPILER could be found.
```

**原因：** 系统未安装 C++ 编译器或构建工具

**解决方案：**

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install build-essential cmake

# 验证安装
g++ --version
cmake --version
```

**macOS:**
```bash
# 安装 Xcode Command Line Tools
xcode-select --install

# 或使用 Homebrew
brew install cmake
```

**Windows:**
- 安装 [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/)
- 或安装 [MinGW-w64](https://www.mingw-w64.org/)
- 推荐使用 Visual Studio 2019 或更高版本

### 智能指针编译错误：shared_from_this 未定义

**错误信息：**
```
error: 'shared_from_this' was not declared in this scope
  163 |             n->parent = shared_from_this();
```

**原因：** 使用 `shared_from_this()` 的类必须继承 `std::enable_shared_from_this`

**解决方案：**

```cpp
// 错误写法
class Node {
private:
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;
public:
    void setNext(const std::shared_ptr<Node>& n) {
        next = n;
        if (n) {
            n->parent = shared_from_this();  // 编译错误！
        }
    }
};

// 正确写法
class Node : public std::enable_shared_from_this<Node> {
private:
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;
public:
    void setNext(const std::shared_ptr<Node>& n) {
        next = n;
        if (n) {
            n->parent = shared_from_this();  // 正确
        }
    }
};
```

**说明：**
- `std::enable_shared_from_this` 提供了 `shared_from_this()` 方法
- 该方法允许类在成员函数中获取指向自身的 `std::shared_ptr`
- 需要包含头文件 `<memory>`

### make_unique 数组默认构造函数错误

**错误信息：**
```
error: no matching function for call to 'Resource::Resource()
  1085 |     { return unique_ptr<_Tp>(new remove_extent_t<_Tp>[__num]()); }
```

**原因：** `make_unique<T[]>()` 需要 T 有默认构造函数

**解决方案：**

**方案 1：使用 vector 替代（推荐）**
```cpp
// 错误写法
std::unique_ptr<Resource[]> arr = std::make_unique<Resource[]>(3);

// 正确写法
std::vector<std::unique_ptr<Resource>> arr;
arr.push_back(std::make_unique<Resource>("Resource-1", 101));
arr.push_back(std::make_unique<Resource>("Resource-2", 102));
arr.push_back(std::make_unique<Resource>("Resource-3", 103));
```

**方案 2：添加默认构造函数**
```cpp
class Resource {
public:
    Resource() : name("Default"), value(0) {
        // 默认构造函数
    }

    Resource(const std::string& n, int v) : name(n), value(v) {
        // 带参数的构造函数
    }

    // ...
};

// 现在可以使用
std::unique_ptr<Resource[]> arr = std::make_unique<Resource[]>(3);
```

**说明：**
- `vector<unique_ptr>` 更灵活，可以控制每个元素的初始化
- 如果确实需要数组智能指针，确保类有默认构造函数

---

## 编译和构建

### CMake 配置失败

**问题：** CMake 无法生成构建文件

**排查步骤：**

1. 检查 CMake 版本
```bash
cmake --version
# 推荐 3.16+ (项目要求)
```

2. 清理构建目录重试
```bash
rm -rf build
mkdir build
cd build
cmake ..
```

3. 查看详细错误信息
```bash
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON
```

### 多线程编译警告

**问题：** 使用 `make -j$(nproc)` 时出现警告

**原因：** 某些编译器或系统不支持多核编译

**解决方案：**
```bash
# 减少并行数
make -j4

# 或使用单线程
make
```

### 链接错误：找不到 pthread

**错误信息：**
```
undefined reference to pthread_create
```

**原因：** 系统需要链接 pthread 库（CMake 通常自动处理）

**解决方案：**

如果 CMake 未自动处理，在 `CMakeLists.txt` 中添加：
```cmake
find_package(Threads REQUIRED)
target_link_libraries(your_target Threads::Threads)
```

---

## 代码问题

### 资源泄漏警告

**症状：** 程序运行正常，但有资源未释放的警告

**常见原因：**
1. 未使用智能指针管理动态内存
2. 循环引用导致引用计数不为零
3. 异常发生时资源未正确释放

**检查方法：**

使用 Valgrind（Linux）：
```bash
valgrind --leak-check=full ./build/your_program
```

使用 AddressSanitizer（GCC/Clang）：
```bash
g++ -fsanitize=address -g main.cpp -o program
./program
```

**解决方法：**
- 使用 `std::unique_ptr` 或 `std::shared_ptr` 管理资源
- 循环引用场景使用 `std::weak_ptr`
- 遵循 RAII 原则（资源获取即初始化）

### 模板编译错误：SFINAE

**错误信息：**
```
error: no matching function for call to 'xxx'
```

**原因：** 模板实例化失败，SFINAE 规则阻止了某些重载

**排查：**
1. 检查模板参数类型
2. 查看完整的错误信息，理解 SFINAE 原因
3. 使用 `static_assert` 提供更清晰的错误信息

**示例：**
```cpp
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
process(T value) {
    // 只接受整数类型
    return value * 2;
}

// 使用 C++20 Concepts 更清晰
template <typename T>
requires std::is_integral_v<T>
T process(T value) {
    return value * 2;
}
```

### 性能测试常见错误

**问题：** 性能测试结果显示不符合预期（如拷贝比移动快）

**常见原因：**

1. **测试条件不公平**
   - 不同测试包含不同数量的构造函数调用
   - 一个测试循环10000次，另一个只循环1次

2. **隐藏的额外开销**
   - 测试包含了非目标操作的时间（如对象创建）
   - 没有排除I/O输出时间

3. **测试方法不当**
   - 测试人造场景而非真实使用场景
   - 循环次数太少或数据量太小

**正确做法：**

```cpp
// ❌ 错误示例 - 测试不公平
void bad_test() {
    BigData temp("Test", 100);  // 只创建1次
    for (int i = 0; i < 10000; ++i) {
        BigData copy = temp;  // 10000次拷贝
    }

    for (int i = 0; i < 10000; ++i) {
        BigData data("Test", 100);  // 10000次构造！
        BigData moved = std::move(data);
    }
    // 结果：拷贝更快，因为少调用了9999次构造函数
}

// ✅ 正确示例 - 测试真实场景
void good_test() {
    // 测试1: vector扩容时的拷贝开销
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        for (int i = 0; i < 10000; ++i) {
            vec.push_back(BigData("Data", 10000));  // 扩容时触发拷贝
        }
    }
    auto copy_time = std::chrono::high_resolution_clock::now() - start;

    // 测试2: 预分配+移动
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(10000);  // 预分配避免扩容
        for (int i = 0; i < 10000; ++i) {
            vec.push_back(BigData("Data", 10000));  // 移动，无拷贝
        }
    }
    auto move_time = std::chrono::high_resolution_clock::now() - start;
    // 结果：移动更快，符合预期
}
```

**最佳实践：**
- ✅ 只对比要测量的操作，确保测试条件公平
- ✅ 测试真实使用场景（如vector操作）
- ✅ 使用静默模式减少输出干扰
- ✅ 循环次数和数据量足够大以体现差异
- ✅ 多次运行取平均值
- ✅ 测量前热身，避免冷启动影响

---

## 寻求帮助

如果以上 FAQ 没有解决你的问题：

1. 查看各语言目录下的 `README.md`
2. 查看 [CLAUDE.md](./CLAUDE.md) 了解项目结构和 AI 协作指南
3. 检查 GitHub Issues 是否有类似问题
4. 提交新的 Issue，包含：
   - 错误信息
   - 操作系统信息
   - 编译器版本（`g++ --version` / `cmake --version`）
   - 复现步骤

---

## 贡献

如果你解决了新的问题，欢迎提交 PR 到 FAQ 中，帮助其他开发者！
