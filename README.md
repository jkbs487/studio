# Studio - AI原生代码实验室

一个用于研究编程语言特性的实验室代码库。

## 项目目标

- **语言特性研究** - 探索各种编程语言的独特特性
- **AI原生设计** - 让AI助手能够高效理解和操作代码库

## 目录结构

```
studio/
├── CLAUDE.md             # AI协作指南（给AI助手的上下文）
├── README.md             # 项目说明（本文档）
├── FAQ.md                # 常见问题和解决方案
├── experiments/          # 实验代码目录
│   └── _template/        # 实验模板
├── languages/            # 语言特性研究（核心）
│   ├── python/           # Python 实验
│   │   ├── decorators/   # 装饰器
│   │   ├── async/        # 异步编程
│   │   ├── metaclass/    # 元类
│   │   └── generators/   # 生成器
│   └── cpp/              # C++ 实验
│       ├── templates/    # 模板
│       ├── smart-pointers/# 智能指针
│       ├── lambda/       # Lambda 表达式
│       └── move-semantics/# 移动语义
├── tools/                # 辅助工具脚本
└── notes/                # 研究笔记
```

## 快速开始

### 运行 Python 实验

```bash
python languages/python/decorators/main.py
```

### 运行 C++ 实验

**首次运行前，请确保已安装编译环境：**

```bash
# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install build-essential cmake

# 验证安装
g++ --version && cmake --version
```

**构建和运行：**

```bash
cd languages/cpp
cmake -B build
cmake --build build
./build/lambda/lambda
```

详见各语言目录下的 `README.md`。

## 常见问题

遇到问题？查看 [FAQ.md](./FAQ.md) 获取常见问题的解决方案，包括：
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
