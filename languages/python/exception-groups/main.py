#!/usr/bin/env python3
"""
Python Exception Groups 实验

本实验探索 Python 3.11 引入的 Exception Groups 特性：
- ExceptionGroup 和 BaseExceptionGroup
- except* 语法
- 处理并发任务中的多个异常
- 实用场景：asyncio 并发任务
"""

import sys
import asyncio


def print_header(title: str):
    print(f"\n{'=' * 50}")
    print(f"  {title}")
    print('=' * 50)


# 检查版本
if sys.version_info < (3, 11):
    print("错误: Exception Groups 需要 Python 3.11+")
    print(f"当前版本: {sys.version}")
    sys.exit(1)


# ============ 基础用法 ============

def basic_usage():
    print_header("1. 基础用法 - 创建异常组")

    # 创建一个异常组
    eg = ExceptionGroup("Multiple errors", [
        ValueError("Invalid value"),
        TypeError("Type mismatch"),
        RuntimeError("Runtime issue")
    ])

    print(f"异常组: {eg}")
    print(f"消息: {eg.message}")
    print(f"异常数量: {len(eg.exceptions)}")

    # 遍历子异常
    print("\n子异常:")
    for i, exc in enumerate(eg.exceptions):
        print(f"  {i + 1}. {type(exc).__name__}: {exc}")


# ============ except* 语法 ============

def except_star_syntax():
    print_header("2. except* 语法")

    def process_data(data):
        results = []
        for key, value in data.items():
            if not isinstance(value, (int, float)):
                raise TypeError(f"{key} must be numeric, got {type(value).__name__}")
            if value < 0:
                raise ValueError(f"{key} must be positive, got {value}")
            results.append(value * 2)
        return results

    test_data = {
        "a": 10,
        "b": -5,      # ValueError
        "c": "hello", # TypeError
        "d": 20
    }

    try:
        process_data(test_data)
    except* ValueError as eg:
        print("捕获到 ValueError 异常组:")
        for exc in eg.exceptions:
            print(f"  - {exc}")
    except* TypeError as eg:
        print("捕获到 TypeError 异常组:")
        for exc in eg.exceptions:
            print(f"  - {exc}")


# ============ 选择性处理子异常 ============

def selective_handling():
    print_header("3. 选择性处理子异常")

    eg = ExceptionGroup("Group", [
        ValueError("invalid value"),
        TypeError("type error"),
        ValueError("another value error"),
    ])

    # 使用 .subgroup() 筛选特定类型的异常
    value_errors = eg.subgroup(ValueError)
    if value_errors:
        print(f"ValueError 异常 ({len(value_errors.exceptions)} 个):")
        for exc in value_errors.exceptions:
            print(f"  - {exc}")

    # 使用 .split() 按类型分组
    matched, unmatched = eg.split(TypeError)
    print(f"\n匹配 TypeError: {matched}")
    print(f"未匹配: {unmatched}")


# ============ asyncio 并发任务 ============

async def fetch_data(name: str, should_fail: bool = False):
    """模拟异步获取数据"""
    await asyncio.sleep(0.1)
    if should_fail:
        raise RuntimeError(f"{name} failed to fetch")
    return f"{name}: data"


async def async_exception_group():
    print_header("4. asyncio 并发任务中的异常组")

    # 创建多个并发任务
    tasks = [
        asyncio.create_task(fetch_data("API1", should_fail=False)),
        asyncio.create_task(fetch_data("API2", should_fail=True)),
        asyncio.create_task(fetch_data("API3", should_fail=True)),
        asyncio.create_task(fetch_data("API4", should_fail=False)),
    ]

    results = await asyncio.gather(*tasks, return_exceptions=True)

    # 检查结果
    success_count = 0
    errors = []

    for i, result in enumerate(results):
        if isinstance(result, Exception):
            errors.append(result)
            print(f"  任务 {i + 1} 失败: {result}")
        else:
            success_count += 1
            print(f"  任务 {i + 1} 成功: {result}")

    print(f"\n成功: {success_count}, 失败: {len(errors)}")

    # 使用 except* 处理
    if errors:
        try:
            raise ExceptionGroup("Task errors", errors)
        except* RuntimeError as eg:
            print(f"\n使用 except* 捕获 {len(eg.exceptions)} 个 RuntimeError:")


# ============ 实用模式 ============

def practical_pattern():
    print_header("5. 实用模式 - 并行任务批量处理")

    def worker(id: int):
        """模拟可能失败的工作函数"""
        if id % 3 == 0:
            raise ValueError(f"Worker {id} got invalid input")
        if id % 7 == 0:
            raise RuntimeError(f"Worker {id} encountered runtime error")
        return f"Worker {id} completed"

    workers = list(range(1, 11))

    # 收集结果和错误
    results = []
    errors = []

    for w in workers:
        try:
            results.append(worker(w))
        except Exception as e:
            errors.append(e)

    print(f"成功: {len(results)}")
    for r in results:
        print(f"  ✓ {r}")

    if errors:
        print(f"\n失败: {len(errors)}")
        # 重新抛出为异常组
        try:
            raise ExceptionGroup("Worker errors", errors)
        except* ValueError as eg:
            print(f"\n处理 ValueError ({len(eg.exceptions)} 个):")
            for e in eg.exceptions:
                print(f"  - {e}")
        except* RuntimeError as eg:
            print(f"\n处理 RuntimeError ({len(eg.exceptions)} 个):")
            for e in eg.exceptions:
                print(f"  - {e}")


async def main_async():
    await async_exception_group()


def main():
    print("Python Exception Groups 实验")
    print(f"Python 版本: {sys.version}")

    basic_usage()
    except_star_syntax()
    selective_handling()

    # 异步实验
    asyncio.run(main_async())

    practical_pattern()

    print("\n" + "=" * 50)
    print("  实验完成!")
    print("=" * 50)


if __name__ == "__main__":
    main()
