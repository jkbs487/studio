# AI助手指南

这是专门为AI助手（如Claude、GPT-4等）提供的上下文文档，帮助AI更好地理解和操作此代码库。

## 项目定位

这是一个**AI原生实验室代码库**，用于：
1. 研究各种编程语言的特性
2. 评估和优化AI生成的代码
3. 探索人机协作的编程模式

## AI行为准则

### 基本原则
- **保持简洁** - 实验代码应专注于演示特定概念
- **添加元数据** - 所有新实验必须包含元数据头
- **记录来源** - 标注代码是人类编写还是AI生成
- **可复现性** - 确保实验可以独立运行

### 代码风格
- 优先使用各语言的标准风格指南
- 添加必要的注释说明意图
- 避免过度工程化

## 目录约定

### experiments/
存放独立的实验项目。每个实验应：
- 包含 `meta.json` 元数据文件
- 自包含，可独立运行
- 有清晰的 README 或注释

### languages/
按语言组织的特性研究：
```
languages/
└── typescript/
    ├── type-system/      # 类型系统实验
    ├── generics/         # 泛型研究
    └── patterns/         # 设计模式实现
```

### ai-generated/
AI生成代码相关：
- `prompts/` - 使用的提示词模板
- `outputs/` - AI生成结果
- `evaluations/` - 质量评估报告

## 元数据规范

每个实验的 `meta.json` 格式：

```json
{
  "name": "实验名称",
  "description": "简短描述",
  "created": "2026-03-12",
  "language": "typescript",
  "tags": ["generics", "type-inference"],
  "ai_participation": "full|partial|none",
  "ai_model": "claude-3-opus|gpt-4|...",
  "source": "human|ai|hybrid"
}
```

## 常见任务指南

### 创建新实验
1. 复制 `experiments/_template/` 目录
2. 更新 `meta.json`
3. 编写实验代码
4. 添加必要的依赖和运行说明

### 评估AI生成代码
1. 在 `ai-generated/prompts/` 记录使用的提示词
2. 在 `ai-generated/outputs/` 保存生成结果
3. 在 `ai-generated/evaluations/` 编写评估报告

### 添加语言研究
1. 在 `languages/` 下创建语言目录
2. 按主题组织子目录
3. 每个主题包含示例和说明

## 工具使用

`tools/` 目录包含辅助脚本：

| 脚本 | 用途 |
|------|------|
| `new-experiment.ts` | 创建新实验 |
| `evaluate-code.ts` | 评估代码质量 |
| `sync-meta.ts` | 同步元数据 |

## 注意事项

- 不要修改 `_template/` 目录中的模板
- 保持实验之间的独立性
- 及时更新元数据
- 大型实验考虑添加 `.gitignore`
