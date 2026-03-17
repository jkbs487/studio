# AI助手指南

这是专门为AI助手提供的上下文文档，帮助AI更好地理解和操作此代码库。

## 项目定位

这是一个**AI原生实验室代码库**，用于：
1. 研究各种编程语言的**最新特性**（C++20/23、Python 3.12+、Go 1.18+、Rust 2021等）
2. 探索人机协作的编程模式

**核心原则**：
- 代码应尽可能使用各语言的最新稳定标准
- 版本要求根据实验实际使用的特性标注最低版本，而非统一最高版本

## 当前项目结构

```
studio/
├── CLAUDE.md              # AI助手指南（本文档）
├── README.md              # 项目说明
├── FAQ.md                 # 常见问题和解决方案
├── package.json           # 项目配置
├── languages/             # 语言特性研究（核心）
│   ├── python/            # Python 示例
│   │   ├── README.md      # Python 实验总览
│   │   ├── decorators/    # 装饰器
│   │   ├── async/         # 异步编程
│   │   ├── metaclass/     # 元类
│   │   └── generators/    # 生成器
│   ├── cpp/               # C++ 示例 (C++20/23)
│   │   ├── README.md      # C++ 实验总览
│   │   ├── CMakeLists.txt # CMake 构建配置
│   │   ├── templates/     # 模板
│   │   ├── smart-pointers/# 智能指针
│   │   ├── lambda/        # Lambda 表达式
│   │   ├── move-semantics/# 移动语义
│   │   ├── concurrency/   # 并发编程
│   │   └── ranges/        # Ranges (C++20)
│   └── go/                # Go 示例
│       ├── README.md      # Go 实验总览
│       └── goroutines/    # 并发编程
├── tools/                 # 辅助工具（Python）
│   └── list_experiments.py# 列出所有实验
└── notes/                 # 研究笔记
```

## AI行为准则

### 基本原则
- **保持简洁** - 每个实验专注于演示一个核心概念
- **添加元数据** - 每个实验必须包含 `meta.json`
- **记录来源** - 标注代码是人类编写还是AI生成
- **可复现性** - 确保实验可以独立运行
- **注释清晰** - 关键逻辑添加注释说明意图

### 代码组织

每个语言特性实验应包含：

**Python 实验：**
```
languages/{language}/{feature}/
├── meta.json    # 元数据（必需）
├── README.md    # 说明文档
└── main.py      # 实验代码
```

**C++ 实验：**
```
languages/{language}/{feature}/
├── meta.json          # 元数据（必需）
├── README.md          # 说明文档
├── main.cpp           # 实验代码
└── CMakeLists.txt     # CMake 构建配置（推荐）
```

**Go 实验：**
```
languages/{language}/{feature}/
├── meta.json    # 元数据（必需）
├── README.md    # 说明文档
└── main.go      # 实验代码
```

每个语言目录还应包含：
- `README.md` - 该语言实验的总览，列出所有实验

C++ 顶层目录应包含：
- `CMakeLists.txt` - 统一构建配置，包含所有子实验

## 元数据规范

每个实验的 `meta.json` 格式：

```json
{
  "name": "实验名称",
  "description": "简短描述",
  "created": "2026-03-12",
  "language": "python",
  "language_version": "3.10+",
  "tags": ["decorator", "meta-programming"],
  "ai_participation": "full|partial|none",
  "ai_model": "claude-3-sonnet|gpt-4|...",
  "source": "human|ai|hybrid"
}
```

### 元数据字段说明

| 字段 | 必需 | 说明 |
|------|------|------|
| name | 是 | 实验名称 |
| description | 是 | 实验描述 |
| created | 是 | 创建日期 YYYY-MM-DD |
| language | 是 | 编程语言 |
| language_version | 是 | 语言版本要求（如 "C++20"、"3.10+"） |
| tags | 是 | 标签数组 |
| ai_participation | 是 | AI参与程度：full/partial/none |
| ai_model | 否 | 使用的AI模型 |
| source | 是 | 代码来源：human/ai/hybrid |

### 语言版本标注规则

**核心原则：根据实验实际使用的特性标注最低版本要求**

每个实验的版本要求应基于代码中实际使用的语言特性，而不是统一使用最高版本。

#### Python 版本标注参考

| 实验类型 | 最低版本 | 关键特性 |
|----------|----------|----------|
| 基础特性 | 3.3+ | `yield from` |
| 元编程 | 3.6+ | `__init_subclass__` |
| 异步编程 | 3.7+ | `asyncio.run()` |
| 类型增强 | 3.10+ | `ParamSpec`、`None` 类型提示 |
| 新语法 | 3.12+ | match-case、f-string 增强等 |

#### C++ 版本标注参考

| 实验类型 | 最低版本 | 关键特性 |
|----------|----------|----------|
| 基础现代特性 | C++11 | 右值引用、移动语义、智能指针 |
| 并发 | C++11/17 | 线程、原子操作、scoped_lock |
| Lambda | C++14 | 泛型 Lambda |
| 新特性 | C++20 | Concepts、Ranges、协程 |

#### Go 版本标注参考

| 实验类型 | 最低版本 | 关键特性 |
|----------|----------|----------|
| 并发编程 | 1.18+ | goroutine、channel、泛型 |
| 错误处理 | 1.13+ | 错误包装、Unwrap |
| 泛型 | 1.18+ | 类型参数、约束 |

#### 标注位置

1. **README.md 标题**: `# 实验名称 \`语言版本\``（如 `# Python 装饰器 \`Python 3.10+\``）
2. **README.md 开头**: 添加 `> **语言版本**: xxx` 标注
3. **meta.json**: `language_version` 字段

## 工具使用

Python 脚本位于 `tools/` 目录：

```bash
# 列出所有实验
python tools/list_experiments.py
```

## 常见任务指南

### 添加新语言特性实验

**步骤：**
1. 创建目录：`languages/{language}/{feature}/`
2. 创建 `meta.json` 元数据
3. 创建 `README.md` 说明文档
4. 创建实验代码（如 `main.py` 或 `main.cpp`）
5. 如是 C++，创建 `CMakeLists.txt` 构建配置
6. 确保代码可直接运行
7. 更新语言目录的 `README.md`，添加新实验到列表

**Python 示例：**
```bash
python tools/new_experiment.py python-async-patterns python
# 手动移动到 languages/python/async-patterns/
```

**C++ 示例：**
创建 `languages/cpp/async/` 目录，包含：
- `meta.json` - 实验元数据
- `README.md` - 实验说明
- `main.cpp` - 可编译运行的 C++ 代码
- `CMakeLists.txt` - CMake 配置

然后更新 `languages/cpp/CMakeLists.txt` 和 `languages/cpp/README.md`。

### 研究笔记

在 `notes/` 目录添加研究心得：
```markdown
# 标题

日期：YYYY-MM-DD
标签：#tag1 #tag2

## 背景

## 发现

## 代码示例

## 参考
```

## AI协作提示词模板

### 创建新实验

**Python 实验：**
```
创建一个 Python 语言特性实验，主题是 {feature}。

要求：
1. 包含 meta.json 元数据
2. 包含 README.md 说明
3. 包含可直接运行的 main.py 代码
4. 代码包含详细注释
5. 演示 {feature} 的核心概念和用法
6. 使用 Python 3.12+ 最新语法特性

反面提示：
- 不要添加不必要的依赖
- 不要生成超出实验范围的代码
- 不要使用项目不支持的语言版本
```

**C++ 实验：**
```
创建一个 C++ 语言特性实验，主题是 {feature}。

要求：
1. 包含 meta.json 元数据（language: "cpp"）
2. 包含 README.md 说明，包含 Windows/Linux/macOS 编译运行说明
3. 包含可直接编译运行的 main.cpp 代码
4. 包含 CMakeLists.txt 构建配置
5. 代码包含详细注释
6. 演示 {feature} 的核心概念和用法
7. 使用 C++20 标准（条件允许时使用 C++23 特性）
8. 代码应跨平台兼容
9. 避免 using namespace std，显式使用 std:: 前缀

反面提示：
- 不要添加非标准库依赖
- 不要生成仅限特定编译器的代码
- 不要忽略跨平台兼容性
```

**Go 实验：**
```
创建一个 Go 语言特性实验，主题是 {feature}。

要求：
1. 包含 meta.json 元数据（language: "go"）
2. 包含 README.md 说明
3. 包含可直接运行的 main.go 代码
4. 代码包含详细注释
5. 演示 {feature} 的核心概念和用法
6. 使用 Go 1.18+ 特性

反面提示：
- 不要添加不必要的依赖
- 不要生成超出实验范围的代码
- 不要使用项目不支持的语言版本
```

### 评估代码质量
```
评估以下代码的质量：

1. 代码正确性
2. 代码风格
3. 可维护性
4. 性能表现
5. 安全性

给出改进建议。
```

## 注意事项

**通用：**
- 实验代码应自包含，可独立运行
- 保持实验之间的独立性
- 及时更新元数据
- 避免过度工程化

**Python 特定：**
- **使用最新标准**：Python 3.12+ 语法，使用新特性如类型提示、match-case、f-string 增强等
- 优先使用类型注解和 `typing` 模块的最新特性

**C++ 特定：**
- **使用最新标准**：默认 C++20，条件允许时使用 C++23 特性（如 `std::expected`、Deducing this 等）
- 使用 CMake 作为主要构建方式，确保跨平台兼容
- 提供 Windows/Linux/macOS 的编译运行说明
- 对于需要线程的实验，正确链接线程库
- 在 README 中说明编译器要求和依赖
- 避免 `using namespace std`，始终显式使用 `std::` 前缀（防止命名冲突和潜在 bug）
- 新特性优先：优先使用 `std::format` 替代 `printf`/`iostream`，`std::jthread` 替代 `std::thread` 等

**Go 特定：**
- **使用最新标准**：Go 1.18+，充分利用泛型等新特性
- 并发编程使用 `sync` 包和 `context` 包
- 生产者-消费者模式使用独立的 WaitGroup 分别等待
- 错误处理遵循 Go 惯例：`if err != nil`

**跨平台兼容：**
- 确保代码在 Windows、Linux、macOS 上都能编译运行
- Windows 支持：MSVC、MinGW-w64、Clang
- 路径处理使用跨平台方式（避免硬编码路径分隔符）
- 提供 CMake 配置以简化构建流程

## 实践案例：创建 C++ 实验集合

### 案例背景
用户要求创建 C++ 语言特性实验，涵盖现代 C++ 核心特性。

### 执行过程

**1. 确定实验主题**
选择四个核心特性：
- 模板
- 智能指针
- Lambda 表达式
- 移动语义

后续新增：
- 并发编程
- Ranges (C++20)

**2. 创建目录结构**
```
languages/cpp/
├── templates/
├── smart-pointers/
├── lambda/
└── move-semantics/
```

**3. 为每个实验创建文件**
每个实验包含：
- `meta.json` - 元数据
- `README.md` - 说明文档
- `main.cpp` - 实验代码
- `CMakeLists.txt` - 构建配置

**4. 创建语言总览**
- `languages/cpp/README.md` - 列出所有实验、快速开始指南
- `languages/cpp/CMakeLists.txt` - 顶层构建配置

**5. 跨平台支持**
- 在 README 中提供 Windows/Linux/macOS 的编译运行说明
- CMake 配置支持多种生成器（Visual Studio、Unix Makefiles 等）
- 手动编译命令也提供多平台版本

**6. 迭代优化**
用户反馈后优化：
- 添加 CMake 支持（统一跨平台构建）
- 简化文档，CMake 方式优先
- 保留手动编译选项作为备选

### 关键经验

**代码组织：**
- 每个实验独立可运行
- 统一的元数据格式
- 一致的文档结构

**构建系统：**
- CMake 是跨平台的最佳选择
- 顶层 CMakeLists.txt 管理所有实验
- 每个实验有独立的 CMakeLists.txt

**文档编写：**
- 总览 README 快速索引所有实验
- 每个 README 包含完整的运行说明
- 多平台命令并列展示

**用户体验：**
- CMake 一键构建所有实验
- IDE 集成（VS Code、Visual Studio、CLion）
- 渐进式学习路径建议

### 最佳实践总结

1. **从简到繁**：先创建基础文件，再优化
2. **跨平台考虑**：Windows 用户多，文档要覆盖
3. **构建系统**：CMake 统一管理，避免平台特定脚本
4. **文档层次**：总览 → 实验 → 详细说明
5. **迭代改进**：根据用户反馈持续优化

## 实践案例：编译错误处理经验总结

### 关键经验

**编译环境检查：**
- 首次编译 C++ 时，先检查编译器是否安装
- Ubuntu/Debian：`build-essential` 包含完整工具链
- 验证命令：`g++ --version` 和 `cmake --version`

**编译错误处理流程：**
1. 仔细阅读错误信息，定位问题根源
2. 理解错误上下文，而非盲目修改代码
3. 查阅文档和 FAQ 获取相关解决方案
4. 修复后编译验证，确保不引入新问题

**C++ 智能指针最佳实践：**
- 使用 `vector<unique_ptr>` 替代 `unique_ptr<T[]>` 更灵活
- 类需要 `shared_from_this()` 时，必须继承 `enable_shared_from_this`
- 智能指针的循环引用使用 `weak_ptr` 解决
- 优先考虑 RAII 原则，避免手动管理资源

**C++ 并发编程最佳实践：**
- 死锁避免：使用 `std::scoped_lock` (C++17) 或 `std::lock` + `adopt_lock`
- 线程安全单例：使用 C++11 局部静态变量
- 条件变量：始终使用带谓词的 `wait()` 重载
- 原子操作：适合简单计数器，复杂操作仍需锁保护

**C++ Ranges 注意事项：**
- 视图是惰性求值，每次迭代都会重新计算
- 避免歧义：std 和 std::ranges 的算法需明确命名空间
- macOS 默认 clang (libc++) 对 Ranges 支持不完整
- 视图不可复制，只能移动

**测试验证：**
- 编译成功后立即运行程序验证功能
- 检查输出是否符合预期
- 确保资源正确释放（析构函数调用）
- 使用工具检查内存泄漏（Valgrind、AddressSanitizer）

### 错误处理原则

| 原则 | 说明 |
|------|------|
| 理解优先 | 先理解错误原因，再寻找解决方案 |
| 查阅文档 | 优先查看 FAQ 和语言规范 |
| 最小改动 | 只修改必要的部分，避免过度修改 |
| 验证完整 | 修复后全面测试，确保功能正常 |
| 记录经验 | 将新问题补充到 FAQ 中 |

**注意：** 具体的错误信息和代码示例请参考 [FAQ.md](./FAQ.md)。

## 实践案例：Go 并发编程经验总结

### 关键经验

**生产者-消费者模式：**
- 使用独立的 `WaitGroup` 分别等待生产者和消费者
- 生产者完成后才能关闭 channel
- 关闭 channel 后消费者才能退出
- 避免混用同一个 WaitGroup 导致 deadlock

**Channel 使用：**
- 无缓冲 channel：同步通信，发送和接收必须同时就绪
- 有缓冲 channel：异步通信，发送者在缓冲区满时阻塞
- 关闭 channel 后，接收者会收到零值和 `false`
- 使用 `for range` 遍历已关闭的 channel

**Context 使用：**
- 使用 `context.Background()` 作为根上下文
- 使用 `context.WithCancel` 创建可取消上下文
- 使用 `context.WithTimeout` 设置超时
- 传递 context 作为函数第一个参数

**Sync 包：**
- `WaitGroup`：等待一组 goroutine 完成
- `Mutex`：互斥锁，保护共享资源
- `RWMutex`：读写锁，读多写少场景
- `Once`：确保函数只执行一次

### 最佳实践总结

| 场景 | 推荐方案 |
|------|----------|
| 等待多个 goroutine | `sync.WaitGroup` |
| 生产者-消费者 | 独立的 WaitGroup + close channel |
| 共享资源保护 | `sync.Mutex` 或 `sync.RWMutex` |
| 超时控制 | `context.WithTimeout` |
| 取消操作 | `context.WithCancel` |
| 原子计数 | `sync/atomic` |

## 实践案例：C++ 协程编程经验总结

### 核心概念

**协程返回对象机制：**
- 协程函数的返回类型决定查找哪个 `promise_type`
- `promise_type::get_return_object()` 负责创建返回对象
- 协程体内不需要显式 `return`，返回值由编译器自动生成

**Lazy vs Eager 协程：**
- Lazy：`initial_suspend()` 返回 `suspend_always`，创建时不执行，需要手动 `resume()`
- Eager：`initial_suspend()` 返回 `suspend_never`，创建时立即执行

### 关键经验

**协程输出不完整：**
- `await_suspend` 返回 `void` 时，协程暂停后不会自动恢复
- 需要在 `await_suspend` 中手动调用 `h.resume()` 恢复协程
- 或让 `await_suspend` 返回 `false` 让协程立即恢复

**协程异常处理：**
- 异常通过 `unhandled_exception()` 存储在 `std::exception_ptr` 中
- Generator 的 `next()` 方法需要检查并重新抛出异常
- 使用 `std::rethrow_exception()` 重新抛出原始异常（C++11 特性）

**协程生命周期管理：**
- 使用 RAII 包装协程句柄，确保析构时调用 `destroy()`
- 禁止复制，允许移动语义
- Task 对象必须在协程执行期间保持有效

**promise_type 必需成员：**
```cpp
struct promise_type {
    ReturnObject get_return_object();  // 必需
    auto initial_suspend();            // 必需
    auto final_suspend() noexcept;     // 必需
    void return_void();                // 必需（或 return_value）
    void unhandled_exception();        // 必需
    auto yield_value(T);               // 可选（使用 co_yield 时）
};
```

### 设计模式

**Generator vs OptionalGenerator：**
- Generator：`next()` 返回 `bool`，需要调用 `current()` 获取值（传统迭代器风格）
- OptionalGenerator：`next()` 返回 `std::optional<T>`，一步获取值和状态（现代 functional 风格）

**自定义 awaitable：**
```cpp
struct SyncSleepAwaitable {
    std::chrono::milliseconds duration;
    
    bool await_ready() const { return false; }
    void await_suspend(std::coroutine_handle<> h) const {
        std::this_thread::sleep_for(duration);
        h.resume();  // 手动恢复协程
    }
    void await_resume() const {}
};
```

### 最佳实践总结

| 场景 | 推荐方案 |
|------|----------|
| 生成器模式 | `Generator<T>` 或 `OptionalGenerator<T>` |
| 异步任务 | `Task` + `co_await` |
| 惰性执行 | Lazy Task（`suspend_always`） |
| 立即执行 | Eager Task（`suspend_never`） |
| 异常传递 | `std::exception_ptr` + `std::rethrow_exception` |
| 资源管理 | RAII 包装协程句柄 |

