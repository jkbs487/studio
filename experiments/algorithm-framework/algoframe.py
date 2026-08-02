"""
算法验证框架核心模块。

提供三个核心 API：
- @solve(name, desc)：注册一个算法（装饰器）
- Case：一个测试用例（args / kwargs / expected / tol）
- register_cases(name, cases, bench=None)：为算法绑定测试用例与基准用例

完整用法见 solutions/example.py。
"""

from __future__ import annotations

import math
import time
import traceback
from dataclasses import dataclass, field
from typing import Any, Callable, Optional

# 算法注册表：name -> Solution
REGISTRY: dict[str, Solution] = {}


@dataclass
class Case:
    """一个测试用例。

    args:     传给算法的位置参数
    kwargs:   传给算法的关键字参数
    expected: 期望输出（基准测试时可为 None，不校验）
    name:     用例名称（可选，便于定位失败用例）
    tol:      数值容差；设置后按容差递归比较（支持嵌套 list/tuple 中的浮点数）
    """

    args: tuple = ()
    kwargs: dict = field(default_factory=dict)
    expected: Any = None
    name: str = ""
    tol: Optional[float] = None


@dataclass
class Solution:
    """一个已注册的算法。"""

    name: str
    func: Callable
    desc: str = ""
    cases: list[Case] = field(default_factory=list)
    bench_cases: list[Case] = field(default_factory=list)


def solve(name: str, desc: str = ""):
    """注册一个算法。

    用法:
        @solve("two-sum", "两数之和：返回和为 target 的两个下标")
        def two_sum(nums, target):
            ...
    """

    def decorator(func: Callable):
        REGISTRY[name] = Solution(name=name, func=func, desc=desc)
        return func

    return decorator


def register_cases(name: str, cases: list[Case], bench: Optional[list[Case]] = None) -> None:
    """为已注册的算法绑定测试用例，可选绑定基准用例（bench 不校验正确性）。"""
    if name not in REGISTRY:
        raise KeyError(
            f"算法 '{name}' 未注册。请先使用 @solve('{name}') 注册，再绑定用例。"
        )
    REGISTRY[name].cases = list(cases)
    if bench is not None:
        REGISTRY[name].bench_cases = list(bench)


def _close(a: Any, b: Any, tol: float) -> bool:
    """按容差递归比较两个值（支持嵌套 list/tuple 中的浮点数）。"""
    if isinstance(a, (int, float)) and isinstance(b, (int, float)):
        return math.isclose(a, b, rel_tol=tol, abs_tol=tol)
    if isinstance(a, (list, tuple)) and isinstance(b, (list, tuple)):
        return len(a) == len(b) and all(_close(x, y, tol) for x, y in zip(a, b))
    return a == b


def _fmt(value: Any) -> str:
    """格式化输出，避免超长输入刷屏。"""
    text = repr(value)
    return text if len(text) <= 200 else text[:197] + "..."


def run_solution(solution: Solution) -> tuple[int, int, float]:
    """运行单个算法的全部用例。

    返回 (通过数, 用例总数, 总耗时秒)。
    """
    passed = 0
    total = 0
    started = time.perf_counter()

    for idx, case in enumerate(solution.cases, 1):
        total += 1
        name = case.name or f"case-{idx}"
        try:
            result = solution.func(*case.args, **case.kwargs)
        except Exception:
            print(f"  [FAIL] {name}: 执行抛出异常")
            traceback.print_exc()
            continue

        if case.tol is not None:
            ok = _close(result, case.expected, case.tol)
        else:
            ok = result == case.expected

        if ok:
            passed += 1
            print(f"  [PASS] {name}")
        else:
            print(f"  [FAIL] {name}")
            print(f"         输入: {_fmt(case.args if case.args else case.kwargs)}")
            print(f"         期望: {_fmt(case.expected)}")
            print(f"         实际: {_fmt(result)}")

    elapsed = time.perf_counter() - started
    return passed, total, elapsed


def bench_solution(solution: Solution) -> None:
    """对基准用例计时（不校验正确性），取 3 次运行的最快耗时。"""
    for case in solution.bench_cases:
        name = case.name or "bench"
        # 预热一次，排除首次分配开销
        solution.func(*case.args, **case.kwargs)

        runs = 3
        times = []
        for _ in range(runs):
            t0 = time.perf_counter()
            solution.func(*case.args, **case.kwargs)
            times.append(time.perf_counter() - t0)

        print(f"  [BENCH] {name}: 最快 {min(times) * 1000:.3f} ms (共 {runs} 次)")
