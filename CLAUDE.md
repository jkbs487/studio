# AI助手指南

这是专门为AI助手提供的上下文文档，帮助AI更好地理解和操作此代码库。

## 项目定位

这是一个**AI原生实验室代码库**，用于：
1. 研究各种编程语言的特性
2. 评估和优化AI生成的代码
3. 探索人机协作的编程模式

## 当前项目结构

```
studio/
├── CLAUDE.md              # AI助手指南（本文档）
├── README.md              # 项目说明
├── package.json           # 项目配置
├── experiments/           # 独立实验（通用）
│   └── _template/         # 实验模板
├── languages/             # 语言特性研究（核心）
│   └── python/            # Python 示例
│       ├── decorators/    # 装饰器
│       ├── async/         # 异步编程
│       ├── metaclass/     # 元类
│       └── generators/    # 生成器
├── ai-generated/          # AI生成代码研究
│   ├── prompts/           # 提示词模板
│   ├── outputs/           # 生成输出
│   └── evaluations/       # 效果评估
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
```
languages/{language}/{feature}/
├── meta.json    # 元数据（必需）
├── README.md    # 说明文档
└── main.py      # 实验代码
```

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

1. 创建目录：`languages/{language}/{feature}/`
2. 创建 `meta.json` 元数据
3. 创建 `README.md` 说明文档
4. 创建 `main.py` 实验代码
5. 确保代码可直接运行

示例：
```bash
python tools/new_experiment.py typescript-generics typescript
# 手动移动到 languages/typescript/generators/
```

### 评估AI生成代码

1. 在 `ai-generated/prompts/` 保存使用的提示词
2. 在 `ai-generated/outputs/` 保存生成结果
3. 在 `ai-generated/evaluations/` 编写评估报告

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
```
创建一个 {language} 语言特性实验，主题是 {feature}。

要求：
1. 包含 meta.json 元数据
2. 包含 README.md 说明
3. 包含可直接运行的 main.py 代码
4. 代码包含详细注释
5. 演示 {feature} 的核心概念和用法
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

- 实验代码应自包含，可独立运行
- 保持实验之间的独立性
- 及时更新元数据
- 使用 Python 3.8+ 语法
- 避免过度工程化
