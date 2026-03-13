# Python 装饰器 `Python 3.10+`

探索 Python 装饰器的各种用法和模式。

> **语言版本**: Python 3.10 或更高（需要 `ParamSpec` 支持）

## 运行

```bash
python languages/python/decorators/main.py
```

## 内容

### 基础函数装饰器
- 使用 `@functools.wraps` 保留原函数信息

### 带参数的装饰器
- 装饰器工厂模式
- 重试装饰器 `@retry(max_attempts=3)`

### 类装饰器
- 使用 `__call__` 实现
- 计数装饰器 `CountCalls`

### 装饰器链
- 多个装饰器叠加
- 执行顺序

### 常用装饰器模式
- 计时装饰器 `@timer`
- 缓存装饰器 `@memoize`
- 日志装饰器 `@log_calls`

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码
