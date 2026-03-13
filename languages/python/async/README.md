# Python 异步编程 `Python 3.7+`

探索 Python 的异步编程特性和模式。

> **语言版本**: Python 3.7 或更高（需要 `asyncio.run()` 支持）

## 运行

```bash
python languages/python/async/main.py
```

## 内容

### 基础协程
- `async def` 定义协程
- `await` 等待协程完成

### 并发执行
- `asyncio.gather()` 并发执行多个任务
- `asyncio.create_task()` 创建任务
- `asyncio.wait()` 等待任务完成

### 异步上下文管理器
- `__aenter__` / `__aexit__`
- `@asynccontextmanager` 装饰器

### 异步生成器
- `AsyncGenerator` 类型
- `async for` 迭代

### 异步迭代器
- `__aiter__` / `__anext__`
- 自定义异步迭代器

### 超时和取消
- `asyncio.wait_for()` 超时控制
- `task.cancel()` 取消任务
- `CancelledError` 异常处理

### 异步队列
- `asyncio.Queue`
- 生产者-消费者模式

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码
