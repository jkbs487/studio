# Studio - AI原生代码实验室

一个用于研究**最新版本**编程语言特性的实验室代码库。

## 项目目标

- **语言特性研究** - 探索各种编程语言的最新特性（C++20/23、Python 3.12+、Rust 2021等）
- **AI原生设计** - 让AI助手能够高效理解和操作代码库
- **前沿技术** - 始终使用各语言的最新稳定标准

## 语言版本要求

各实验根据实际使用的特性标注最低版本要求：

### Python 实验版本要求

| 实验 | 最低版本 | 关键特性 |
|------|----------|----------|
| generators | 3.3+ | `yield from` |
| metaclass | 3.6+ | `__init_subclass__` |
| async | 3.7+ | `asyncio.run()` |
| decorators | 3.10+ | `ParamSpec` |
| type-hints | 3.9+ | 类型提示系统 |
| pattern-matching | 3.10+ | `match/case` |
| dataclasses | 3.7+ | `@dataclass` |
| exception-groups | 3.11+ | `except*` |

### C++ 实验版本要求

| 实验 | 最低版本 | 关键特性 |
|------|----------|----------|
| move-semantics | C++11 | 右值引用、移动语义 |
| smart-pointers | C++11 | 智能指针 |
| concurrency | C++11/17 | 线程、原子操作、scoped_lock |
| lambda | C++14 | 泛型 Lambda |
| templates | C++20 | Concepts |
| ranges | C++20 | Ranges 库 |

### Go 实验版本要求

| 实验 | 最低版本 | 关键特性 |
|------|----------|----------|
| goroutines | 1.18+ | goroutine、channel、sync、context |

## 目录结构

```
studio/
├── CLAUDE.md             # AI协作指南（给AI助手的上下文）
├── README.md             # 项目说明（本文档）
├── FAQ.md                # 常见问题和解决方案
├── languages/            # 语言特性研究（核心）
│   ├── python/           # Python 实验
│   │   ├── decorators/   # 装饰器
│   │   ├── async/        # 异步编程
│   │   ├── metaclass/    # 元类
│   │   ├── generators/   # 生成器
│   │   ├── type-hints/   # 类型提示
│   │   ├── pattern-matching/# 模式匹配
│   │   ├── dataclasses/  # 数据类
│   │   └── exception-groups/# 异常组
│   ├── cpp/              # C++ 实验 (C++20/23)
│   │   ├── templates/    # 模板
│   │   ├── smart-pointers/# 智能指针
│   │   ├── lambda/       # Lambda 表达式
│   │   ├── move-semantics/# 移动语义
│   │   ├── concurrency/  # 并发编程
│   │   └── ranges/       # Ranges (C++20)
│   └── go/               # Go 实验
│       └── goroutines/   # 并发编程
├── tools/                # 辅助工具脚本
│   └── list_experiments.py # 列出所有实验
└── notes/                # 研究笔记
```

## 快速开始

### 查看所有实验

使用 `list_experiments.py` 快速浏览所有实验：

```bash
python tools/list_experiments.py
```

输出示例：
```
已有实验 (15 个):

名称: go-goroutines
语言: go (1.18+)
描述: 探索 Go 并发编程：goroutine、channel、select、sync 包、上下文取消
标签: [goroutine, channel, concurrency, concurrent]
创建时间: 2026-03-13
来源: AI生成
AI参与: 完全AI
路径: languages/go/goroutines
```

### 运行 Python 实验

```bash
python languages/python/decorators/main.py
```

### 运行 C++ 实验

**首次运行前，请确保已安装支持 C++20 的编译环境：**

```bash
# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install build-essential cmake

# macOS
xcode-select --install
brew install cmake

# 验证安装（需要 GCC 10+ 或 Clang 13+）
g++ --version && cmake --version
```

**构建和运行：**

```bash
cd languages/cpp
cmake -B build
cmake --build build
./build/concurrency/concurrency   # 并发编程
./build/ranges/ranges             # C++20 Ranges
```

详见各语言目录下的 `README.md`。

### 运行 Go 实验

```bash
# 确保已安装 Go 1.18+
go version

# 运行实验
go run languages/go/goroutines/main.go
```

## 常见问题

遇到问题？查看 [FAQ.md](./FAQ.md) 获取常见问题的解决方案，包括：
- Go 编译运行问题
- C++ 编译器配置问题
- 智能指针编译错误
- 构建系统问题
- 代码调试技巧

## AI协作指南

本项目专为AI辅助开发设计。详见 [CLAUDE.md](./CLAUDE.md) 了解如何与AI助手高效协作。

## AI原生特征

| 特性 | 说明 |
|------|------|
| CLAUDE.md | 专门给AI助手的上下文指南 |
| 元数据规范 | 记录每个实验的AI参与度、生成来源 |
| 模块化设计 | 每个实验独立，便于AI理解和操作 |

## 许可证

MIT License
