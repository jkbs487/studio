# 异常组 (Exception Groups) `Python 3.11+`

本实验探索 Python 3.11 引入的 Exception Groups 和 `except*` 语法。

> **注意**: 需要 Python 3.11+ 版本

## 运行

```bash
python languages/python/exception-groups/main.py
```

## 内容

### 基础用法
- `ExceptionGroup` 创建异常组
- 遍历子异常

### except* 语法
- 同时捕获多种异常类型
- 按类型分发处理

### 选择性处理
- `.subgroup()` 筛选特定类型
- `.split()` 按类型分组

### asyncio 并发任务
- 并发任务中的异常收集
- `gather(*tasks, return_exceptions=True)`

### 实用模式
- 批量任务处理
- 错误聚合与分类处理

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码
