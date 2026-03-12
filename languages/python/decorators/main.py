#!/usr/bin/env python3
"""
Python 装饰器实验
探索装饰器的各种用法和模式
"""

import time
import functools
from typing import Callable, TypeVar, ParamSpec, Any

P = ParamSpec("P")
T = TypeVar("T")


# ============================================
# 1. 基础函数装饰器
# ============================================

def log_calls(func: Callable[P, T]) -> Callable[P, T]:
    """记录函数调用的装饰器"""
    @functools.wraps(func)
    def wrapper(*args: P.args, **kwargs: P.kwargs) -> T:
        print(f"[LOG] 调用 {func.__name__}({args}, {kwargs})")
        result = func(*args, **kwargs)
        print(f"[LOG] {func.__name__} 返回: {result}")
        return result
    return wrapper


# ============================================
# 2. 带参数的装饰器
# ============================================

def retry(max_attempts: int = 3, delay: float = 1.0):
    """带参数的重试装饰器"""
    def decorator(func: Callable[P, T]) -> Callable[P, T]:
        @functools.wraps(func)
        def wrapper(*args: P.args, **kwargs: P.kwargs) -> T:
            last_error = None
            for attempt in range(1, max_attempts + 1):
                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    last_error = e
                    print(f"[RETRY] {func.__name__} 第 {attempt} 次失败: {e}")
                    if attempt < max_attempts:
                        time.sleep(delay)
            raise last_error
        return wrapper
    return decorator


# ============================================
# 3. 类装饰器
# ============================================

class CountCalls:
    """使用类实现的计数装饰器"""

    def __init__(self, func: Callable[P, T]):
        self.func = func
        self.count = 0
        functools.update_wrapper(self, func)

    def __call__(self, *args: P.args, **kwargs: P.kwargs) -> T:
        self.count += 1
        print(f"[COUNT] {self.func.__name__} 已调用 {self.count} 次")
        return self.func(*args, **kwargs)


# ============================================
# 4. 计时装饰器
# ============================================

def timer(func: Callable[P, T]) -> Callable[P, T]:
    """测量函数执行时间的装饰器"""
    @functools.wraps(func)
    def wrapper(*args: P.args, **kwargs: P.kwargs) -> T:
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        print(f"[TIMER] {func.__name__} 耗时: {end - start:.6f} 秒")
        return result
    return wrapper


# ============================================
# 5. 缓存装饰器（记忆化）
# ============================================

def memoize(func: Callable[P, T]) -> Callable[P, T]:
    """简单的记忆化装饰器"""
    cache: dict[tuple, T] = {}

    @functools.wraps(func)
    def wrapper(*args: P.args, **kwargs: P.kwargs) -> T:
        # 创建可哈希的缓存键
        key = (args, frozenset(kwargs.items()))

        if key in cache:
            print(f"[MEMO] {func.__name__} 从缓存返回")
            return cache[key]

        result = func(*args, **kwargs)
        cache[key] = result
        return result

    return wrapper


# ============================================
# 6. 装饰器链
# ============================================

def bold(func: Callable[P, str]) -> Callable[P, str]:
    """添加粗体标记"""
    @functools.wraps(func)
    def wrapper(*args: P.args, **kwargs: P.kwargs) -> str:
        return f"**{func(*args, **kwargs)}**"
    return wrapper


def italic(func: Callable[P, str]) -> Callable[P, str]:
    """添加斜体标记"""
    @functools.wraps(func)
    def wrapper(*args: P.args, **kwargs: P.kwargs) -> str:
        return f"*{func(*args, **kwargs)}*"
    return wrapper


# ============================================
# 测试函数
# ============================================

@log_calls
def add(a: int, b: int) -> int:
    """简单加法函数"""
    return a + b


@retry(max_attempts=3, delay=0.5)
def unstable_function(should_fail: bool = True) -> str:
    """模拟不稳定的函数"""
    if should_fail and unstable_function.attempts < 2:  # type: ignore
        unstable_function.attempts += 1  # type: ignore
        raise ConnectionError("连接失败")
    return "成功!"


unstable_function.attempts = 0  # type: ignore


@CountCalls
def greet(name: str) -> str:
    """问候函数"""
    return f"你好, {name}!"


@timer
def slow_function(n: int) -> int:
    """模拟耗时操作"""
    time.sleep(0.1)
    return sum(range(n))


@memoize
def fibonacci(n: int) -> int:
    """斐波那契数列（带缓存）"""
    if n < 2:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)


@bold
@italic
def get_text() -> str:
    """获取文本（应用多个装饰器）"""
    return "Hello, World!"


# ============================================
# 主函数
# ============================================

def main():
    print("=" * 60)
    print("Python 装饰器实验")
    print("=" * 60)

    # 1. 基础装饰器
    print("\n1. 基础函数装饰器 (log_calls):")
    result = add(3, 5)
    print(f"结果: {result}")

    # 2. 带参数的装饰器
    print("\n2. 带参数的装饰器 (retry):")
    try:
        result = unstable_function(should_fail=True)
        print(f"最终结果: {result}")
    except Exception as e:
        print(f"所有重试失败: {e}")

    # 3. 类装饰器
    print("\n3. 类装饰器 (CountCalls):")
    greet("张三")
    greet("李四")
    greet("王五")
    print(f"总调用次数: {greet.count}")

    # 4. 计时装饰器
    print("\n4. 计时装饰器 (timer):")
    result = slow_function(10000)
    print(f"计算结果: {result}")

    # 5. 缓存装饰器
    print("\n5. 缓存装饰器 (memoize):")
    print(f"fibonacci(10) = {fibonacci(10)}")
    print(f"fibonacci(10) = {fibonacci(10)} (缓存)")
    print(f"fibonacci(15) = {fibonacci(15)}")

    # 6. 装饰器链
    print("\n6. 装饰器链 (bold + italic):")
    print(f"结果: {get_text()}")

    print("\n" + "=" * 60)
    print("实验完成!")


if __name__ == "__main__":
    main()
