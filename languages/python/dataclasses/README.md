# 数据类 (dataclasses) `Python 3.7+`

本实验探索 Python `dataclasses` 的用法，大幅减少样板代码。

## 运行

```bash
python languages/python/dataclasses/main.py
```

## 内容

### 基础用法
- 自动生成 `__init__`, `__repr__`, `__eq__`
- 带默认值的字段

### 字段选项 (field)
- `field(default=...)` 默认值
- `field(default_factory=...)` 可变默认值
- `field(init=False)` 不参与初始化

### 排序比较 (order=True)
- 自动生成 `__lt__`, `__le__`, `__gt__`, `__ge__`

### 冻结对象 (frozen=True)
- 不可变对象，类似 namedtuple

### post_init 钩子
- 在 `__init__` 后执行自定义逻辑

### 实用模式
- 配置类、数据传输对象(DTO)
- 状态对象

### 转换
- `asdict()` 转换为字典

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码
