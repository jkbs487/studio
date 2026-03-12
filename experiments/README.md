# 实验目录

此目录存放独立的实验项目。

## 创建新实验

```bash
# 复制模板
cp -r experiments/_template experiments/your-experiment-name

# 编辑元数据
vim experiments/your-experiment-name/meta.json

# 开始实验
cd experiments/your-experiment-name
```

## 实验规范

每个实验必须包含：

1. `meta.json` - 元数据文件（必需）
2. `README.md` - 实验说明（推荐）
3. 实验代码文件

## 元数据字段

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| name | string | 是 | 实验名称 |
| description | string | 是 | 简短描述 |
| created | string | 是 | 创建日期 YYYY-MM-DD |
| language | string | 是 | 主要编程语言 |
| tags | string[] | 是 | 标签列表 |
| ai_participation | enum | 是 | AI参与程度：full/partial/none |
| ai_model | string | 否 | 使用的AI模型 |
| source | enum | 是 | 代码来源：human/ai/hybrid |
