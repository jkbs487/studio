# AI助手指南

这是专门为AI助手提供的上下文文档，帮助AI更好地理解和操作此代码库。

## 项目定位

这是一个**AI原生实验室代码库**，用于：
1. 研究各种编程语言的**最新特性**（C++20/23、Python 3.12+、Rust 2021等）
2. 探索人机协作的编程模式

**核心原则：始终使用各语言的最新稳定标准**

- C++：使用 C++20，条件允许时使用 C++23 特性
- Python：使用 Python 3.12+ 语法
- Rust：使用 Rust 2021 Edition

## 当前项目结构

```
studio/
├── CLAUDE.md              # AI助手指南（本文档）
├── README.md              # 项目说明
├── FAQ.md                 # 常见问题和解决方案
├── package.json           # 项目配置
├── experiments/           # 独立实验（通用）
│   └── _template/         # 实验模板
├── languages/             # 语言特性研究（核心）
│   ├── python/            # Python 示例
│   │   ├── README.md      # Python 实验总览
│   │   ├── decorators/    # 装饰器
│   │   ├── async/         # 异步编程
│   │   ├── metaclass/     # 元类
│   │   └── generators/    # 生成器
│   └── cpp/               # C++ 示例 (C++20/23)
│       ├── README.md      # C++ 实验总览
│       ├── CMakeLists.txt # CMake 构建配置
│       ├── templates/     # 模板
│       ├── smart-pointers/# 智能指针
│       ├── lambda/        # Lambda 表达式
│       ├── move-semantics/# 移动语义
│       ├── concurrency/   # 并发编程
│       └── ranges/        # Ranges (C++20)
├── tools/                 # 辅助工具（Python）
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
| tags | 是 | 标签数组 |
| ai_participation | 是 | AI参与程度：full/partial/none |
| ai_model | 否 | 使用的AI模型 |
| source | 是 | 代码来源：human/ai/hybrid |

## 工具使用

Python 脚本位于 `tools/` 目录：

```bash
# 创建新实验
python tools/new_experiment.py <name> [language]

# 评估代码质量
python tools/evaluate_code.py <path>

# 同步元数据索引
python tools/sync_meta.py
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

