# 常见问题 (FAQ)

本文档收集了在使用 Studio 代码库时常见的问题和解决方案。

## 目录

- [Python 相关](#python-相关)
- [Go 相关](#go-相关)
- [C++ 相关](#c-相关)
- [编译和构建](#编译和构建)
- [代码问题](#代码问题)
- [性能对比实验](#性能对比实验)

---

## Python 相关

### Python 版本要求

**问题：** 运行 Python 代码时提示语法错误

**原因：** 项目使用 Python 3.10+ 的特性（如 match/case、dataclass 参数）

**解决方案：**
```bash
# 检查 Python 版本
python3 --version

# 建议使用 3.10 或更高版本
# Ubuntu/Debian:
sudo apt install python3.11

# macOS:
brew install python@3.11
```

### Python 未找到

**问题：** `python: command not found`

**解决方案：**

```bash
# macOS
brew install python3

# Linux
sudo apt install python3

# 或使用 pyenv 管理多版本
brew install pyenv
pyenv install 3.11
pyenv global 3.11
```

### 虚拟环境使用

**问题：** 依赖冲突或全局环境污染

**解决方案：**

```bash
# 创建虚拟环境
python3 -m venv venv

# 激活
source venv/bin/activate  # Linux/macOS
venv\Scripts\activate    # Windows

# 安装依赖
pip install -r requirements.txt

# 停用
deactivate
```

### Python 异步编程注意事项

**问题：** 异步函数未正确执行

**原因：** 未使用 `await` 或 `asyncio.run()`

**解决方案：**

```python
# ❌ 错误：异步函数未等待
async def fetch_data():
    return await get_data()

result = fetch_data()  # 返回协程对象，不是结果

# ✅ 正确：使用 asyncio.run
async def main():
    result = await fetch_data()
    print(result)

asyncio.run(main())

# 或使用 asyncio.get_event_loop()
loop = asyncio.get_event_loop()
result = loop.run_until_complete(fetch_data())
```

### 类型提示常见错误

**问题：** 类型提示不生效或运行时错误

**原因：** Python 类型提示是静态检查，不影响运行时

**解决方案：**

```python
# 类型提示不会在运行时强制类型
def greet(name: str) -> str:
    return f"Hello, {name}"

# 传入错误类型不会报错，只会有 IDE 警告
result = greet(123)  # 运行正常，但 IDE 会警告

# 如需运行时检查，使用 typing 或 pydantic
from pydantic import BaseModel

class Person(BaseModel):
    name: str
    age: int

# 运行时自动验证
person = Person(name="Alice", age="25")  # 会报错，类型不匹配
```

### 装饰器执行顺序

**问题：** 装饰器执行结果与预期不符

**原因：** 装饰器是从下往上执行的

**解决方案：**

```python
def decorator_a(func):
    print("decorator_a")
    return func

def decorator_b(func):
    print("decorator_b")
    return func

@decorator_a
@decorator_b
def my_func():
    pass

# 输出:
# decorator_b  # 先执行
# decorator_a  # 后执行
# 效果等同于: decorator_a(decorator_b(my_func))
```

### 生成器内存效率

**问题：** 处理大数据时内存不足

**原因：** 使用列表而非生成器

**解决方案：**

```python
# ❌ 错误：一次性加载所有数据
def get_items():
    return [item for item in range(1000000)]

# ✅ 正确：使用生成器
def get_items():
    for item in range(1000000):
        yield item

# ✅ 或使用生成器表达式
items = (item for item in range(1000000))
```

### match/case 使用注意

**问题：** match/case 在 Python 3.10 以下报错

**错误信息：**
```
SyntaxError: invalid syntax
```

**解决方案：**
```bash
# 确保使用 Python 3.10+
python3 --version  # 应该是 3.10 或更高

# 或使用兼容写法
if isinstance(x, int):
    # old style matching
    pass
```

---

## Go 相关

### 运行时检测：Go 未安装

**问题：** 运行 Go 代码时提示命令未找到

**错误信息：**
```
zsh: command not found: go
```

**解决方案：**

**macOS (使用 Homebrew):**
```bash
brew install go

# 验证安装
go version
```

**Linux:**
```bash
# 下载安装包
wget https://go.dev/dl/go1.21.linux-amd64.tar.gz
sudo rm -rf /usr/local/go
sudo tar -C /usr/local -xzf go1.21.linux-amd64.tar.gz

# 添加到 PATH
echo 'export PATH=$PATH:/usr/local/go/bin' >> ~/.bashrc
source ~/.bashrc
```

**Windows:**
- 下载 MSI 安装包：https://go.dev/dl/
- 或使用 Chocolatey: `choco install golang`

### Go 并发编程：生产者-消费者 deadlock

**问题：** 使用 WaitGroup 时出现 deadlock

**错误信息：**
```
fatal error: all goroutines are asleep - deadlock!
```

**原因：** 多个 WaitGroup 混用或生产者/消费者的同步逻辑不正确

**解决方案：**

```go
// ❌ 错误：同一个 WaitGroup 用于生产者和消费者
wg.Add(numProducers)  // 生产者
wg.Add(numConsumers)  // 消费者
wg.Wait()  // 可能导致死锁

// ✅ 正确：分别等待生产者和消费者
var producerWg sync.WaitGroup
var consumerWg sync.WaitGroup

// 启动生产者
for p := 0; p < numProducers; p++ {
    producerWg.Add(1)
    go func(id int) {
        defer producerWg.Done()
        // 生产数据...
        ch <- item
    }(p)
}

// 启动消费者
for c := 0; c < numConsumers; c++ {
    consumerWg.Add(1)
    go func(id int) {
        defer consumerWg.Done()
        for {
            item, ok := <-ch
            if !ok {
                return
            }
            // 消费数据...
        }
    }(c)
}

// 等待生产者完成
producerWg.Wait()
// 关闭通道，通知消费者没有更多数据
close(ch)
// 等待消费者完成
consumerWg.Wait()
```

**关键点：**
- 生产者完成后才能关闭 channel
- 关闭 channel 后消费者才能退出
- 使用两个独立的 WaitGroup 分别等待生产者和消费者

### Go 通道遍历注意事项

**问题：** 遍历已关闭通道时没有输出

**原因：** 遍历已关闭的空通道不会产生任何值

```go
// ✅ 正确：先填充数据，再关闭，再遍历
buffered := make(chan string, 3)
buffered <- "first"
buffered <- "second"
buffered <- "third"
close(buffered)

// 现在遍历会输出所有值
for msg := range buffered {
    fmt.Println(msg)
}

// ❌ 错误：创建空通道后立即关闭，再遍历
ch := make(chan string)
close(ch)
for msg := range ch {  // 不会输出任何内容
    fmt.Println(msg)
}
```

---

## C++ 相关

### C++ 协程编译错误

#### std::future 不支持协程

**错误信息：**
```
error: 'std::coroutine_traits<std::future<int>, int, int>' has no member named 'promise_type'
```

**原因：** C++20 标准库的 `std::future` 没有内置协程支持（需 C++23 或第三方库）

**解决方案：** 自定义返回类型，实现自己的 `promise_type`

```cpp
// ❌ 错误：std::future 不支持协程
std::future<int> asyncAdd(int a, int b) {
    co_return a + b;
}

// ✅ 正确：自定义返回类型
struct SimpleFuture {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        int value = 0;
        SimpleFuture get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return SimpleFuture{h};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { value = v; }
        void unhandled_exception() { std::terminate(); }
    };

    int value() const { return handle.promise().value; }
};

SimpleFuture asyncAdd(int a, int b) {
    co_return a + b;
}
```

#### 协程句柄无法访问 promise

**错误信息：**
```
error: no member named 'promise' in 'std::coroutine_handle<void>'
```

**原因：** 使用了空的协程句柄模板 `std::coroutine_handle<>`，无法访问 promise

**解决方案：** 使用带模板参数的协程句柄

```cpp
// ❌ 错误
std::coroutine_handle<> handle;
handle.promise().value;

// ✅ 正确
std::coroutine_handle<promise_type> handle;
handle.promise().value;
```

#### promise_type 前向声明缺失

**错误信息：**
```
error: use of undeclared identifier 'promise_type'
```

**原因：** 返回类型中 promise_type 未正确声明

**解决方案：** 前向声明 promise_type

```cpp
struct SimpleFuture {
    struct promise_type;  // 前向声明
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        // ...
    };
};
```

#### Lambda 捕获成员变量失败

**错误信息：**
```
error: 'duration' in capture list does not name a variable
```

**原因：** Lambda 不能直接捕获 `this->member`，需要先复制到局部变量

**解决方案：**

```cpp
// ❌ 错误
std::thread([handle, duration]() {
    std::this_thread::sleep_for(duration);  // error
}).detach();

// ✅ 正确：先复制到局部变量
auto d = duration;
std::thread([handle, d]() {
    std::this_thread::sleep_for(d);
}).detach();
```

#### co_yield 序列点警告

**警告信息：**
```
warning: multiple unsequenced modifications to 'i'
```

**原因：** `co_yield i++` 在同一表达式中对 i 进行修改和读取

**解决方案：** 分离 yield 和递增操作

```cpp
// ❌ 错误
co_yield i++;

// ✅ 正确
co_yield i;
++i;
```

### C++ 协程运行时问题

#### 协程输出不完整

**问题：** 协程中 `co_await` 后的代码没有执行

**原因：** 自定义 awaitable 的 `await_suspend` 返回值或行为不正确

**解决方案：**

```cpp
// ❌ 错误：await_suspend 返回 void，协程暂停后不会自动恢复
void await_suspend(std::coroutine_handle<>) const {
    std::this_thread::sleep_for(duration);
}

// ✅ 正确：手动恢复协程
void await_suspend(std::coroutine_handle<> h) const {
    std::this_thread::sleep_for(duration);
    h.resume();  // 手动恢复
}
```

**关键点：**
- `await_suspend` 返回 `void`：协程暂停，需要手动 resume
- `await_suspend` 返回 `true`：协程暂停
- `await_suspend` 返回 `false`：协程立即恢复

#### 协程异常未被捕获

**问题：** 协程中抛出的异常没有被 `try-catch` 捕获

**原因：** Generator 的 `next()` 方法没有检查并重新抛出异常

**解决方案：**

```cpp
// ❌ 错误：next() 不检查异常
bool next() {
    handle.resume();
    return !handle.done();
}

// ✅ 正确：检查并重新抛出异常
bool next() {
    handle.resume();
    if (handle.done()) {
        if (handle.promise().exception) {
            std::rethrow_exception(handle.promise().exception);
        }
        return false;
    }
    return true;
}
```

**说明：**
- 协程异常通过 `unhandled_exception()` 存储在 `std::exception_ptr` 中
- 使用 `std::rethrow_exception()` 重新抛出原始异常
- `std::exception_ptr` 和 `std::rethrow_exception` 是 C++11 特性

#### 协程句柄未销毁

**问题：** 程序运行正常，但存在内存泄漏

**原因：** 协程句柄创建后未调用 `destroy()`

**解决方案：** 使用 RAII 包装协程句柄

```cpp
struct Generator {
    std::coroutine_handle<promise_type> handle;

    ~Generator() {
        if (handle) {
            handle.destroy();
        }
    }

    // 禁止复制
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    // 允许移动
    Generator(Generator&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
};
```

#### Lazy vs Eager 协程

**问题：** 不理解协程何时开始执行

**原因：** 混淆了 `suspend_always` 和 `suspend_never` 的行为

**解决方案：**

```cpp
// Lazy（惰性）协程 - 调用时不执行
struct LazyTask {
    struct promise_type {
        std::suspend_always initial_suspend() { return {}; }  // 先暂停
        // ...
    };
    
    void resume() { handle.resume(); }  // 手动恢复
};

// Eager（急切）协程 - 调用时立即执行
struct EagerTask {
    struct promise_type {
        std::suspend_never initial_suspend() { return {}; }  // 立即开始
        // ...
    };
};
```

| 类型 | `initial_suspend()` | 行为 |
|------|---------------------|------|
| Lazy | `suspend_always` | 创建时不执行，需要手动 `resume()` |
| Eager | `suspend_never` | 创建时立即执行 |

#### promise_type 必需成员

**问题：** 编写协程时不知道 `promise_type` 需要哪些成员

**必需成员：**

```cpp
struct promise_type {
    // 必需：创建返回对象
    ReturnObject get_return_object();
    
    // 必需：协程开始时的行为
    auto initial_suspend();  // 返回 suspend_always 或 suspend_never
    
    // 必需：协程结束时的行为
    auto final_suspend() noexcept;
    
    // 必需（二选一）：协程返回值
    void return_void();      // 用于 co_return; 或无返回语句
    void return_value(T);    // 用于 co_return value;
    
    // 必需：异常处理
    void unhandled_exception();
    
    // 可选：支持 co_yield
    auto yield_value(T);     // 返回 suspend_always 或类似类型
};
```

**编译错误示例：**
```cpp
// 缺少 get_return_object
error: no member named 'get_return_object' in 'promise_type'

// 缺少 yield_value（使用 co_yield 时）
error: no member named 'yield_value' in 'promise_type'
```

#### 协程返回对象机制

**问题：** 为什么协程函数没有 `return` 语句，但能返回指定类型？

**原因：** 协程的返回值由 `promise_type::get_return_object()` 创建

**编译器生成的伪代码：**
```cpp
ReturnObject coroutineFunction() {
    // 编译器自动生成：
    auto& promise = /* 创建 promise */;
    ReturnObject result = promise.get_return_object();  // ← 创建返回值
    co_await promise.initial_suspend();
    // ... 协程体 ...
    return result;  // 返回由 promise 创建的对象
}
```

**关键点：**
- 协程返回类型决定了查找哪个 `promise_type`
- `get_return_object()` 负责创建返回对象
- 协程体内不需要显式 `return`

#### Generator vs OptionalGenerator

**问题：** 两种生成器有什么区别？

**Generator（两步模式）：**
```cpp
// next() 返回 bool，需要调用 current() 获取值
while (gen.next()) {
    std::cout << gen.current();
}
```

**OptionalGenerator（一步模式）：**
```cpp
// next() 直接返回 std::optional<T>
while (auto val = gen.next()) {
    std::cout << *val;
}
```

| 特性 | Generator | OptionalGenerator |
|------|-----------|-------------------|
| `next()` 返回 | `bool` | `std::optional<T>` |
| 获取值 | `current()` | 解引用 `*val` |
| 调用次数 | 两步 | 一步 |
| 风格 | 传统迭代器 | 现代 functional |

### C++ 协程编译器支持

| 编译器 | 最低版本 | 编译选项 |
|--------|----------|----------|
| GCC | 10+ | `-fcoroutines` |
| Clang | 16+ | `-fcoroutines` (实验性) |
| MSVC | 19.14+ | 默认支持 |
| Apple Clang | 14.0+ | `-fcoroutines` |

**验证编译器支持：**
```bash
g++ --version
# 或
clang++ --version
```

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

### std::expected 编译错误

**错误信息：**
```
error: no member named 'map' in 'std::expected<int, std::string>'
```

**原因：** C++23 的 `std::expected` 的 monadic 函数（`map`、`and_then`、`map_error`、`and_else`）是 C++26 特性，当前编译器尚未支持。

**解决方案：** 手动实现链式操作

```cpp
// ❌ 错误：C++26 特性，当前编译器不支持
auto result = exp.map([](int x) { return x * 2; });

// ✅ 正确：手动实现
auto doubled = exp ? std::expected<int, std::string>(*exp * 2) : exp;

// ✅ and_then 手动实现
auto square = [](int x) -> std::expected<int, std::string> {
    if (x > 100) return std::unexpected("Too large");
    return x * x;
};
auto chained = exp ? square(*exp) : exp;

// ✅ and_else 手动实现（fallback）
auto withFallback = exp ? exp : std::expected<int, std::string>(0);
```

### std::expected 使用注意事项

**问题：** 构造 expected 时的错误

**原因：** `std::expected::unexpected` 是静态成员函数，当前编译器可能不支持

**解决方案：**

```cpp
// ❌ 错误
auto e = std::expected<T, E>::unexpected("error");

// ✅ 正确
auto e = std::unexpected(std::string("error"));
// 或
auto e = std::expected<T, E>(std::unexpect_t{}, "error");
```

**模板函数注意事项：**

```cpp
// ❌ 错误：默认构造错误类型
template<typename T, typename E>
std::expected<T, E> safeDivide(T a, T b) {
    if (b == T{}) return std::unexpected(E{});  // E 可能不支持默认构造
}

// ✅ 正确：传入错误值
template<typename T, typename E>
std::expected<T, E> safeDivide(T a, T b, E zeroError) {
    if (b == T{}) return std::unexpected(zeroError);
}

auto r = safeDivide<int, const char*>(10, 0, "Division by zero");
```

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

## 代码问题（通用）

> 以下问题跨越多种语言，或涉及通用编程概念

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

**常见原因：** 测试条件不公平、测试方法不当、循环次数或数据量不足、编译器优化掩盖真实性能

**正确做法：**
- ✅ 确保测试条件公平（相同的数据规模、循环次数）
- ✅ 测试真实使用场景（如vector操作）
- ✅ 循环次数和数据量足够大以体现差异
- ✅ 多次运行取平均值
- ✅ 使用 `-O2` 或更高优化级别

**注意：** 编译器优化（RVO、SSO、copy elision）可能掩盖移动语义和拷贝的性能差异

### emplace_back 使用建议

```cpp
// ✅ 推荐：直接构造
vec.emplace_back("Name", 1000);

// ❌ 避免：创建临时对象
vec.push_back(BigData("Name", 1000));

// ✅ 对象已存在，使用拷贝/移动
BigData obj("Name", 1000);
vec.push_back(obj);              // 拷贝（obj 还要使用）
vec.push_back(std::move(obj));   // 移动（obj 不再使用）
```

**关键点：**
- emplace_back 避免临时对象的创建和销毁
- 对于构造开销大的对象，emplace_back 优势明显
- 对于构造开销小的对象，emplace_back 和 push_back 差异不大
- 不要用 emplace_back 替代 `push_back(已有对象)` 的拷贝

### 并发编程常见问题

**线程对象未 join 或 detach：** 线程对象在销毁前必须调用 `join()` 或 `detach()`

**死锁：** 使用 `std::scoped_lock` 或 `std::lock` 确保多个 mutex 按相同顺序加锁

**竞争条件：** 使用互斥锁 `std::mutex` 或原子操作 `std::atomic`

**条件变量虚假唤醒：** 使用带谓词的 `cv.wait(lock, predicate)` 而非 `cv.wait(lock)`

### Ranges 常见问题

#### 视图不可迭代

**错误：**
```
error: passing 'const xxx_view' as 'this' argument discards qualifiers
```

**原因：** 视图在 const 上下文中不可迭代

**解决方案：**
```cpp
// 将视图复制到 vector
std::vector<int> result;
for (int x : view | std::views::filter(...)) {
    result.push_back(x);
}
```

#### std::ranges 算法歧义

**错误：**
```
error: reference to 'sort' is ambiguous
```

**原因：** std 和 std::ranges 都有同名算法

**解决方案：**
```cpp
// 明确命名空间
std::sort(vec.begin(), vec.end());           // std 算法
std::ranges::sort(vec);                      // ranges 算法
```

#### macOS 编译 Ranges 失败

**问题：** macOS 默认 clang (libc++) 对 C++20 Ranges 支持不完整

**解决方案：**
```bash
# 使用 GCC (需要安装)
g++ -std=c++20 main.cpp -o program

# 或指定 libstdc++
clang++ -std=c++20 -stdlib=libc++ main.cpp  # 可能不支持
```

#### volatile 复合赋值警告

**错误：**
```
warning: compound assignment to object of volatile-qualified type is deprecated
```

**原因：** C++20 中对 volatile 类型使用复合赋值被弃用

**解决方案：**
```cpp
// ❌ 旧写法
volatile size_t total = 0;
total += value;

// ✅ 新写法
volatile size_t total = 0;
total = total + value;
```

---

## 性能对比实验

### 运行 benchmark 性能测试

**问题：** 如何运行 C++/Go/Python 性能对比实验？

**解决方案：**

```bash
cd experiments/benchmark

# 编译并运行（默认）
./run.sh

# 仅编译
./run.sh build

# 仅运行
./run.sh run

# 清理生成文件
./run.sh clean
```

### benchmark 结果不符合预期

**问题：** 性能测试结果与文档不符

参考 [性能测试常见错误](#性能测试常见错误) 部分的建议。检查编译器版本：
```bash
g++ --version
go version
python3 --version
```

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
