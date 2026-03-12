#!/usr/bin/env python3
"""
Python 异步编程实验
探索 async/await 和 asyncio 的各种用法
"""

import asyncio
import time
from typing import AsyncIterator, AsyncGenerator
from contextlib import asynccontextmanager


# ============================================
# 1. 基础协程
# ============================================

async def say_hello(name: str, delay: float) -> str:
    """简单的异步函数"""
    print(f"  开始处理 {name}...")
    await asyncio.sleep(delay)
    print(f"  完成 {name}!")
    return f"你好, {name}!"


# ============================================
# 2. 并发执行
# ============================================

async def fetch_data(url: str, delay: float) -> dict:
    """模拟异步获取数据"""
    print(f"  正在获取 {url}...")
    await asyncio.sleep(delay)
    return {"url": url, "data": f"来自 {url} 的数据"}


async def concurrent_fetch():
    """并发获取多个资源"""
    print("\n并发获取:")
    urls = [
        ("api/users", 0.5),
        ("api/posts", 0.3),
        ("api/comments", 0.4),
    ]

    # 方式1: gather - 并发执行并收集结果
    tasks = [fetch_data(url, delay) for url, delay in urls]
    results = await asyncio.gather(*tasks)

    for result in results:
        print(f"  获取到: {result}")

    return results


async def fetch_with_progress():
    """使用 as_completed 模式的进度追踪"""
    print("\n带进度的获取:")

    async def fetch_with_name(name: str, delay: float):
        await asyncio.sleep(delay)
        return name, delay

    tasks = [
        asyncio.create_task(fetch_with_name("A", 0.5)),
        asyncio.create_task(fetch_with_name("B", 0.3)),
        asyncio.create_task(fetch_with_name("C", 0.7)),
    ]

    done, pending = await asyncio.wait(
        tasks,
        return_when=asyncio.FIRST_COMPLETED
    )

    for task in done:
        name, delay = await task
        print(f"  最先完成: {name} (延迟 {delay}s)")

    # 等待剩余任务
    if pending:
        remaining = await asyncio.gather(*pending)
        for name, delay in remaining:
            print(f"  后续完成: {name} (延迟 {delay}s)")


# ============================================
# 3. 异步上下文管理器
# ============================================

class AsyncDatabase:
    """模拟异步数据库连接"""

    def __init__(self, name: str):
        self.name = name
        self.connected = False

    async def __aenter__(self) -> "AsyncDatabase":
        print(f"  连接到数据库 {self.name}...")
        await asyncio.sleep(0.2)
        self.connected = True
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        print(f"  断开数据库 {self.name}")
        await asyncio.sleep(0.1)
        self.connected = False

    async def query(self, sql: str) -> list:
        if not self.connected:
            raise RuntimeError("数据库未连接")
        await asyncio.sleep(0.1)
        return [f"结果: {sql}"]


@asynccontextmanager
async def async_timer(name: str):
    """异步计时上下文管理器"""
    print(f"  开始: {name}")
    start = time.perf_counter()
    try:
        yield
    finally:
        elapsed = time.perf_counter() - start
        print(f"  结束: {name}, 耗时: {elapsed:.3f}s")


async def demo_async_context():
    """演示异步上下文管理器"""
    print("\n异步上下文管理器:")

    async with AsyncDatabase("main_db") as db:
        results = await db.query("SELECT * FROM users")
        print(f"  查询结果: {results}")

    async with async_timer("异步操作"):
        await asyncio.sleep(0.3)
        print("  执行中...")


# ============================================
# 4. 异步生成器
# ============================================

async def async_range(start: int, end: int, delay: float = 0.1) -> AsyncGenerator[int, None]:
    """异步生成器"""
    for i in range(start, end):
        await asyncio.sleep(delay)
        yield i


async def async_countdown(n: int) -> AsyncIterator[str]:
    """异步倒计时"""
    for i in range(n, 0, -1):
        await asyncio.sleep(0.2)
        yield f"T-{i}"
    yield "发射!"


async def demo_async_generator():
    """演示异步生成器"""
    print("\n异步生成器:")

    print("  异步范围:")
    async for num in async_range(1, 5):
        print(f"    数字: {num}")

    print("  异步倒计时:")
    async for msg in async_countdown(3):
        print(f"    {msg}")


# ============================================
# 5. 异步迭代器
# ============================================

class AsyncCounter:
    """自定义异步迭代器"""

    def __init__(self, start: int, end: int, delay: float = 0.1):
        self.current = start
        self.end = end
        self.delay = delay

    def __aiter__(self):
        return self

    async def __anext__(self) -> int:
        if self.current >= self.end:
            raise StopAsyncIteration
        await asyncio.sleep(self.delay)
        value = self.current
        self.current += 1
        return value


async def demo_async_iterator():
    """演示异步迭代器"""
    print("\n异步迭代器:")

    counter = AsyncCounter(10, 15, 0.15)
    async for num in counter:
        print(f"  计数: {num}")


# ============================================
# 6. 超时和取消
# ============================================

async def long_running_task(seconds: float):
    """模拟长时间运行的任务"""
    print(f"  开始长时间任务 ({seconds}s)...")
    try:
        for i in range(int(seconds * 10)):
            await asyncio.sleep(0.1)
            print(f"  进度: {i + 1}/{int(seconds * 10)}")
    except asyncio.CancelledError:
        print("  任务被取消!")
        raise


async def demo_timeout_and_cancel():
    """演示超时和取消"""
    print("\n超时处理:")

    try:
        # 使用 wait_for 设置超时
        result = await asyncio.wait_for(
            say_hello("延迟任务", 2.0),
            timeout=0.5
        )
    except asyncio.TimeoutError:
        print("  任务超时!")

    print("\n任务取消:")
    task = asyncio.create_task(long_running_task(5.0))
    await asyncio.sleep(0.5)

    print("  取消任务...")
    task.cancel()
    try:
        await task
    except asyncio.CancelledError:
        print("  任务已成功取消")


# ============================================
# 7. 异步队列
# ============================================

async def producer(queue: asyncio.Queue, items: list):
    """生产者"""
    for item in items:
        await asyncio.sleep(0.1)
        await queue.put(item)
        print(f"  [生产者] 放入: {item}")
    await queue.put(None)  # 结束信号


async def consumer(queue: asyncio.Queue, name: str):
    """消费者"""
    while True:
        item = await queue.get()
        if item is None:
            queue.task_done()
            break
        print(f"  [{name}] 处理: {item}")
        await asyncio.sleep(0.2)
        queue.task_done()


async def demo_queue():
    """演示异步队列"""
    print("\n生产者-消费者模式:")
    queue: asyncio.Queue = asyncio.Queue()

    items = ["任务1", "任务2", "任务3", "任务4"]

    # 启动生产者和消费者
    await asyncio.gather(
        producer(queue, items),
        consumer(queue, "消费者A"),
    )


# ============================================
# 主函数
# ============================================

async def main():
    print("=" * 60)
    print("Python 异步编程实验")
    print("=" * 60)

    # 1. 基础协程
    print("\n1. 基础协程:")
    result = await say_hello("世界", 0.3)
    print(f"  返回值: {result}")

    # 2. 并发执行
    print("\n2. 并发执行:")
    await concurrent_fetch()
    await fetch_with_progress()

    # 3. 异步上下文管理器
    print("\n3. 异步上下文管理器:")
    await demo_async_context()

    # 4. 异步生成器
    print("\n4. 异步生成器:")
    await demo_async_generator()

    # 5. 异步迭代器
    print("\n5. 异步迭代器:")
    await demo_async_iterator()

    # 6. 超时和取消
    print("\n6. 超时和取消:")
    await demo_timeout_and_cancel()

    # 7. 异步队列
    print("\n7. 异步队列:")
    await demo_queue()

    print("\n" + "=" * 60)
    print("实验完成!")


if __name__ == "__main__":
    asyncio.run(main())
