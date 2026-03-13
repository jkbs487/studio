# Python 生成器 `Python 3.3+`

探索 Python 生成器和迭代器的各种用法。

> **语言版本**: Python 3.3 或更高（需要 `yield from` 支持）

## 运行

```bash
python languages/python/generators/main.py
```

## 内容

### 基础生成器
- `yield` 关键字
- 无限斐波那契数列

### yield from 委托
- 委托给子生成器
- 扁平化嵌套结构
- 链接多个生成器

### 生成器表达式
- 惰性求值
- 内存效率对比

### 无限序列
- 素数生成器
- 循环生成器
- `itertools.islice`

### 管道模式
- 链式处理
- 过滤、转换、限制

### 协程 (send/throw/close)
- `send()` 发送值
- `throw()` 抛出异常
- `close()` 关闭生成器
- 累加器、平均值计算

### 状态机
- 双向生成器
- 事件驱动状态转换

### 惰性求值
- 按需处理
- 节省资源

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码
