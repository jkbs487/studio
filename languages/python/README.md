# Python 语言特性研究

本目录包含 Python 语言特性的实验和研究。

> **版本说明**: 各实验根据实际使用的特性标注最低版本要求，详见各实验列表

## 实验列表

### 装饰器 (decorators) `Python 3.10+`

探索 Python 装饰器的各种用法和技巧。

**内容：**
- 基础函数装饰器
- 带参数的装饰器
- 类装饰器
- 装饰器链
- 常用装饰器模式（计时、缓存、重试）

**运行：**
```bash
python languages/python/decorators/main.py
```

---

### 异步编程 (async) `Python 3.7+`

探索 Python 的异步编程模型。

**运行：**
```bash
python languages/python/async/main.py
```

---

### 元类 (metaclass) `Python 3.6+`

探索 Python 元类和元编程。

**运行：**
```bash
python languages/python/metaclass/main.py
```

---

### 生成器 (generators) `Python 3.3+`

探索 Python 生成器和迭代器。

**运行：**
```bash
python languages/python/generators/main.py
```

---

## 快速开始

选择感兴趣的实验目录，运行对应的 `main.py` 文件：

```bash
# 进入实验目录
cd languages/python/{experiment-name}

# 运行实验
python main.py
```

## 实验规范

每个实验目录包含：
- `meta.json` - 实验元数据
- `README.md` - 实验说明文档
- `main.py` - 可运行的实验代码

## 相关链接

- [Python 官方文档](https://docs.python.org/zh-cn/3/)
- [CLAUDE.md](../../CLAUDE.md) - AI 协作指南
