---
title: python-generators
description: 探索 Python 生成器：yield、yield from、生成器表达式、管道模式、惰性求值
---

# python-generators

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | python |
| 版本 | 3.3+ |
| 创建时间 | 2026-03-12 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `generator` `yield` `iterator` `lazy-evaluation` |

</div>

## 描述

探索 Python 生成器：yield、yield from、生成器表达式、管道模式、惰性求值

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


## 源码

<details>
<summary>main.py</summary>

```python
#!/usr/bin/env python3
"""
Python 生成器实验
探索生成器和迭代器的高级用法
"""

from typing import Generator, Iterator
from itertools import islice, takewhile, count
import time


# ============================================
# 1. 基础生成器
# ============================================

def simple_range(start: int, end: int, step: int = 1) -> Generator[int, None, None]:
    """简单的范围生成器"""
    current = start
    while current < end:
        yield current
        current += step


def fibonacci() -> Generator[int, None, None]:
    """无限斐波那契数列生成器"""
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b


def demo_basic_generators():
    """演示基础生成器"""
    print("\n1. 基础生成器:")

    print("  简单范围:")
    for num in simple_range(1, 6):
        print(f"    {num}", end=" ")
    print()

    print("  斐波那契前10项:")
    fib = fibonacci()
    first_10 = [next(fib) for _ in range(10)]
    print(f"    {first_10}")


# ============================================
# 2. yield from 委托
# ============================================

def flatten(nested: list) -> Generator:
    """扁平化嵌套列表"""
    for item in nested:
        if isinstance(item, (list, tuple)):
            yield from flatten(item)
        else:
            yield item


def chain_generators(*generators) -> Generator:
    """链接多个生成器"""
    for gen in generators:
        yield from gen


def demo_yield_from():
    """演示 yield from"""
    print("\n2. yield from 委托:")

    # 扁平化嵌套
    nested = [1, [2, 3, [4, 5]], 6, [[7, 8], 9]]
    flat = list(flatten(nested))
    print(f"  扁平化: {flat}")

    # 链接生成器
    gen1 = (x for x in range(3))
    gen2 = (x * 10 for x in range(3))
    gen3 = (x * 100 for x in range(3))
    chained = list(chain_generators(gen1, gen2, gen3))
    print(f"  链接: {chained}")


# ============================================
# 3. 生成器表达式
# ============================================

def demo_generator_expressions():
    """演示生成器表达式"""
    print("\n3. 生成器表达式:")

    # 列表推导 vs 生成器表达式
    numbers = range(10)

    # 列表推导 - 立即计算
    squares_list = [x ** 2 for x in numbers]
    print(f"  列表推导: {squares_list}")

    # 生成器表达式 - 惰性计算
    squares_gen = (x ** 2 for x in numbers)
    print(f"  生成器表达式: {squares_gen}")
    print(f"  转换为列表: {list(squares_gen)}")

    # 内存使用对比
    import sys

    big_list = [x for x in range(10000)]
    big_gen = (x for x in range(10000))

    print(f"  列表内存: {sys.getsizeof(big_list)} bytes")
    print(f"  生成器内存: {sys.getsizeof(big_gen)} bytes")


# ============================================
# 4. 无限序列
# ============================================

def primes() -> Generator[int, None, None]:
    """无限素数生成器"""
    def is_prime(n: int) -> bool:
        if n < 2:
            return False
        for i in range(2, int(n ** 0.5) + 1):
            if n % i == 0:
                return False
        return True

    num = 2
    while True:
        if is_prime(num):
            yield num
        num += 1


def cycle(iterable) -> Generator:
    """无限循环迭代器"""
    saved = list(iterable)
    while saved:
        for item in saved:
            yield item


def demo_infinite_sequences():
    """演示无限序列"""
    print("\n4. 无限序列:")

    # 素数生成器
    print("  前20个素数:")
    prime_gen = primes()
    print(f"    {list(islice(prime_gen, 20))}")

    # 循环生成器
    print("  循环 ABC (取10个):")
    cycler = cycle(['A', 'B', 'C'])
    print(f"    {list(islice(cycler, 10))}")


# ============================================
# 5. 管道模式
# ============================================

def read_lines(filename: str) -> Generator[str, None, None]:
    """读取文件行（模拟）"""
    lines = [
        "Hello World",
        "Python Generators",
        "Yield Expression",
        "Pipeline Pattern",
        "Lazy Evaluation"
    ]
    for line in lines:
        yield line


def filter_lines(lines: Generator[str, None, None], keyword: str) -> Generator[str, None, None]:
    """过滤包含关键字的行"""
    for line in lines:
        if keyword.lower() in line.lower():
            yield line


def transform_lines(lines: Generator[str, None, None]) -> Generator[str, None, None]:
    """转换行为大写"""
    for line in lines:
        yield line.upper()


def limit_lines(lines: Generator[str, None, None], n: int) -> Generator[str, None, None]:
    """限制行数"""
    for i, line in enumerate(lines):
        if i >= n:
            break
        yield line


def demo_pipeline():
    """演示管道模式"""
    print("\n5. 管道模式:")

    # 构建处理管道
    pipeline = (
        limit_lines(
            transform_lines(
                filter_lines(
                    read_lines("dummy.txt"),
                    "e"
                )
            ),
            3
        )
    )

    print("  管道处理结果 (包含'e'的行, 大写, 最多3行):")
    for line in pipeline:
        print(f"    {line}")


# ============================================
# 6. 协程 (send/throw/close)
# ============================================

def accumulator(initial: float = 0) -> Generator[float, float, float]:
    """累加器协程"""
    total = initial
    print(f"  [协程] 初始值: {total}")

    while True:
        try:
            value = yield total
            if value is not None:
                total += value
                print(f"  [协程] 接收 {value}, 总计: {total}")
        except GeneratorExit:
            print(f"  [协程] 关闭, 最终值: {total}")
            raise
        except Exception as e:
            print(f"  [协程] 异常: {e}")


def averaging() -> Generator[float, float, float]:
    """平均值计算协程"""
    total = 0.0
    count = 0

    while True:
        try:
            value = yield total / count if count > 0 else 0
            if value is not None:
                total += value
                count += 1
        except GeneratorExit:
            avg = total / count if count > 0 else 0
            print(f"  [协程] 最终平均值: {avg}")
            raise


def demo_coroutine():
    """演示协程"""
    print("\n6. 协程 (send/throw/close):")

    # 累加器
    print("  累加器:")
    acc = accumulator(10)
    next(acc)  # 启动协程

    acc.send(5)
    acc.send(3)
    result = acc.send(2)
    print(f"  当前结果: {result}")

    acc.close()

    # 平均值
    print("\n  平均值计算:")
    avg = averaging()
    next(avg)

    for val in [10, 20, 30, 40]:
        result = avg.send(val)
        print(f"    发送 {val}, 当前平均: {result:.2f}")

    avg.close()


# ============================================
# 7. 双向生成器
# ============================================

def state_machine() -> Generator[str, str, None]:
    """状态机生成器"""
    state = "IDLE"
    print(f"  [状态机] 初始状态: {state}")

    while True:
        event = yield state
        print(f"  [状态机] 收到事件: {event}, 当前状态: {state}")

        if state == "IDLE":
            if event == "START":
                state = "RUNNING"
        elif state == "RUNNING":
            if event == "PAUSE":
                state = "PAUSED"
            elif event == "STOP":
                state = "IDLE"
        elif state == "PAUSED":
            if event == "RESUME":
                state = "RUNNING"
            elif event == "STOP":
                state = "IDLE"


def demo_state_machine():
    """演示状态机"""
    print("\n7. 状态机:")

    sm = state_machine()
    initial_state = next(sm)
    print(f"  初始状态: {initial_state}")

    events = ["START", "PAUSE", "RESUME", "STOP", "START", "STOP"]

    for event in events:
        state = sm.send(event)
        print(f"  事件 {event} -> 状态 {state}")


# ============================================
# 8. 惰性求值
# ============================================

def lazy_file_processor(filenames: list) -> Generator[dict, None, None]:
    """惰性文件处理器"""
    for filename in filenames:
        # 模拟文件处理
        print(f"  [惰性] 处理文件: {filename}")
        time.sleep(0.1)  # 模拟耗时操作
        yield {
            "filename": filename,
            "size": len(filename) * 100,
            "processed": True
        }


def demo_lazy_evaluation():
    """演示惰性求值"""
    print("\n8. 惰性求值:")

    files = ["data1.csv", "data2.json", "data3.xml", "data4.txt"]

    print("  创建生成器 (不执行):")
    processor = lazy_file_processor(files)
    print(f"  生成器对象: {processor}")

    print("\n  开始消费 (逐步执行):")
    # 只处理前两个
    for i, result in enumerate(processor):
        print(f"  结果: {result}")
        if i >= 1:
            print("  停止消费...")
            break


# ============================================
# 主函数
# ============================================

def main():
    print("=" * 60)
    print("Python 生成器实验")
    print("=" * 60)

    demo_basic_generators()
    demo_yield_from()
    demo_generator_expressions()
    demo_infinite_sequences()
    demo_pipeline()
    demo_coroutine()
    demo_state_machine()
    demo_lazy_evaluation()

    print("\n" + "=" * 60)
    print("实验完成!")


if __name__ == "__main__":
    main()

```

</details>



<style>
.experiment-meta {
  margin: 1rem 0;
}
.experiment-meta table {
  margin: 0;
}
</style>
