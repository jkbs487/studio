# Python 模式匹配 (Pattern Matching) `Python 3.10+`

探索 Python 3.10 引入的 `match/case` 语法。

> **语言版本**: Python 3.10 或更高

## 运行

```bash
python languages/python/pattern-matching/main.py
```

## 内容

### 基础 match
- `match value:` 语法
- `case pattern:` 分支
- `_` 通配符

### 组合模式
- `|` 或运算符
- `case 200 | 201:`

### 序列模式
- 列表/元组解构
- `*rest` 剩余元素

### 映射模式
- 字典解构
- 部分匹配

### 类模式
- `@dataclass` 解构
- 位置参数匹配

### 守卫
- `if` 条件守卫
- `case pattern if condition:`

### 类型匹配
- `int()`, `str()` 类型检查
- `as` 绑定

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码
