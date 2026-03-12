# Studio - AI原生代码实验室

一个用于研究编程语言特性和AI生成代码的实验室代码库。

## 项目目标

- 🧪 **语言特性研究** - 探索各种编程语言的独特特性
- 🤖 **AI生成代码研究** - 评估和优化AI代码生成的质量
- 🔄 **AI原生设计** - 让AI助手能够高效理解和操作代码库

## 目录结构

```
studio/
├── experiments/          # 实验代码目录
│   ├── _template/        # 实验模板（新建实验时复制）
│   └── .meta-schema.json # 实验元数据规范
├── languages/            # 语言特性研究
│   ├── typescript/
│   ├── python/
│   ├── rust/
│   └── ...
├── ai-generated/         # AI生成代码研究
│   ├── prompts/          # 提示词模板库
│   ├── outputs/          # AI生成输出
│   └── evaluations/      # 生成效果评估
├── tools/                # 辅助工具脚本
└── notes/                # 研究笔记
```

## 快速开始

### 创建新实验

1. 复制 `experiments/_template/` 目录
2. 重命名为你的实验名称
3. 填写实验元数据
4. 开始实验

### AI协作指南

本项目专为AI辅助开发设计。详见 [CLAUDE.md](./CLAUDE.md) 了解如何与AI助手高效协作。

## AI原生特征

| 特性 | 说明 |
|------|------|
| CLAUDE.md | 专门给AI助手的上下文指南 |
| 元数据规范 | 记录每个实验的AI参与度、生成来源 |
| 模块化设计 | 每个实验独立，便于AI理解和操作 |
| 提示词库 | 可复用的AI交互模板 |

## 许可证

MIT License
