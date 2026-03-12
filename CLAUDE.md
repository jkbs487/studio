# AI助手指南

这是专门为AI助手提供的上下文文档，帮助AI更好地理解和操作此代码库。

## 项目定位

这是一个**AI原生实验室代码库**，用于：
1. 研究各种编程语言的特性
2. 探索人机协作的编程模式

## 当前项目结构

```
studio/
├── CLAUDE.md              # AI助手指南（本文档）
├── README.md              # 项目说明
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
│   └── cpp/               # C++ 示例
│       ├── README.md      # C++ 实验总览
│       ├── CMakeLists.txt # CMake 构建配置
│       ├── templates/     # 模板
│       ├── smart-pointers/# 智能指针
│       ├── lambda/        # Lambda 表达式
│       └── move-semantics/# 移动语义
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
7. 使用 C++17 或更高标准（推荐 C++20）
8. 代码应跨平台兼容
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
- 使用 Python 3.8+ 语法

**C++ 特定：**
- 使用 CMake 作为主要构建方式，确保跨平台兼容
- 提供 Windows/Linux/macOS 的编译运行说明
- 使用 C++17 或更高标准（推荐 C++20）
- 对于需要线程的实验，正确链接线程库
- 在 README 中说明编译器要求和依赖

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

## 实践案例：编译执行 C++ 实验并修复编译错误

### 案例背景

用户在 Linux 系统上首次尝试编译 C++ 实验项目，遇到编译器缺失和代码编译错误，需要 AI 协助解决问题并验证实验可执行性。

### 问题诊断

**初始错误：**
```
cmake ..
-- The CXX compiler identification is unknown
CMake Error at CMakeLists.txt:2 (project):
  No CMAKE_CXX_COMPILER could be found.
```

**分析：**
- 系统未安装 C++ 编译器
- CMake 无法检测到可用的编译器

### 解决步骤

**1. 环境准备**
```bash
# 安装编译工具链
sudo apt update
sudo apt install -y build-essential cmake

# 验证安装
g++ --version  # g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
cmake --version  # cmake version 3.28.3
```

**2. 清理重建**
```bash
cd languages/cpp
rm -rf build
mkdir build
cd build
cmake ..
```

输出：
```
-- The CXX compiler identification is GNU 13.3.0
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Configuring done (1.2s)
-- Generating done (0.0s)
-- Build files have been written to: /home/.../build
```

**3. 编译发现错误**

执行 `make -j$(nproc)` 时发现 smart-pointers 编译失败：

```
error: 'shared_from_this' was not declared in this scope
  163 |             n->parent = shared_from_this();

error: no matching function for call to 'Resource::Resource()'
 1085 |     { return unique_ptr<_Tp>(new remove_extent_t<_Tp>[__num]()); }
```

**4. 修复编译错误**

**错误 1：`shared_from_this()` 未定义**

问题：`Node` 类使用 `shared_from_this()` 但未继承 `std::enable_shared_from_this`

修复（smart-pointers/main.cpp:145）：
```cpp
// 修改前
class Node {
private:
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;

// 修改后
class Node : public std::enable_shared_from_this<Node> {
private:
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;
```

**错误 2：`make_unique<Resource[]>` 无默认构造函数**

问题：`Resource` 类没有默认构造函数，但 `make_unique<Resource[]>(3)` 需要默认构造

修复（smart-pointers/main.cpp:73-75）：
```cpp
// 修改前
std::unique_ptr<Resource[]> arr = std::make_unique<Resource[]>(3);
std::cout << "创建动态数组" << std::endl;

// 修改后
std::vector<std::unique_ptr<Resource>> arr;
arr.push_back(std::make_unique<Resource>("Array-1", 101));
arr.push_back(std::make_unique<Resource>("Array-2", 102));
arr.push_back(std::make_unique<Resource>("Array-3", 103));
std::cout << "创建动态数组 (使用vector<unique_ptr>): " << arr.size() << " 个元素" << std::endl;
```

**5. 重新编译**
```bash
make -j$(nproc)
```

输出：
```
[ 62%] Building CXX object smart-pointers/CMakeFiles/smart-pointers.dir/main.cpp.o
[100%] Linking CXX executable smart-pointers
[100%] Built target smart-pointers
```

**6. 执行验证**

成功运行所有 4 个实验程序：

```bash
./templates/templates       # C++ 模板实验
./smart-pointers/smart-pointers  # 智能指针实验
./lambda/lambda            # Lambda 表达式实验
./move-semantics/move-semantics  # 移动语义实验
```

所有实验均正常输出，验证了代码正确性。

### 关键经验

**编译环境检查：**
- 首次在系统上编译 C++ 时，先检查编译器是否安装
- Ubuntu/Debian：`build-essential` 包含完整工具链
- 验证命令：`g++ --version` 和 `cmake --version`

**编译错误处理：**
1. 仔细阅读错误信息，定位问题根源
2. `shared_from_this()` 必须与 `std::enable_shared_from_this` 配合使用
3. `make_unique<T[]>()` 要求 T 有默认构造函数，考虑替代方案

**C++ 最佳实践：**
- 使用 `vector<unique_ptr>` 替代 `unique_ptr<T[]>` 更灵活
- 类需要 `shared_from_this()` 时，必须继承 `enable_shared_from_this`
- 智能指针的循环引用使用 `weak_ptr` 解决

**测试验证：**
- 编译成功后立即运行程序验证功能
- 检查输出是否符合预期
- 确保资源正确释放（析构函数调用）

### 错误模式总结

| 错误类型 | 常见原因 | 解决方案 |
|---------|---------|---------|
| 编译器未找到 | 系统未安装编译工具 | 安装 build-essential (Linux) / VS Build Tools (Windows) |
| `shared_from_this` 未定义 | 类未继承 `enable_shared_from_this` | 添加继承 `: public std::enable_shared_from_this<T>` |
| 数组智能指针错误 | 缺少默认构造函数 | 使用 `vector<unique_ptr>` 替代 |

