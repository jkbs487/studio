"""
算法框架使用示例。

演示如何使用 @solve 注册算法、定义 Case 用例、以及 register_cases 绑定测试。

运行: python run.py example-algorithm --bench
"""

from algoframe import Case, register_cases, solve


# --- 算法 1：两数之和 ---


@solve("two-sum", "两数之和：返回和为 target 的两个下标")
def two_sum(nums: list[int], target: int) -> tuple[int, int] | None:
    seen: dict[int, int] = {}
    for i, n in enumerate(nums):
        complement = target - n
        if complement in seen:
            return (seen[complement], i)
        seen[n] = i
    return None


register_cases(
    "two-sum",
    cases=[
        Case(args=([2, 7, 11, 15], 9), expected=(0, 1), name="basic"),
        Case(args=([3, 2, 4], 6), expected=(1, 2), name="no-duplicate"),
        Case(args=([3, 3], 6), expected=(0, 1), name="duplicate"),
        Case(args=([1, 5, 3], 10), expected=None, name="no-solution"),
    ],
    bench=[
        Case(args=(list(range(100_000)), 199_997), name="large-array"),
    ],
)


# --- 算法 2：阶乘（演示数值算法 + 容差） ---


@solve("factorial", "阶乘：返回 n!")
def factorial(n: int) -> int:
    if n < 0:
        raise ValueError("n must be non-negative")
    result = 1
    for i in range(2, n + 1):
        result *= i
    return result


register_cases(
    "factorial",
    cases=[
        Case(args=(0,), expected=1, name="zero"),
        Case(args=(1,), expected=1, name="one"),
        Case(args=(5,), expected=120, name="five"),
        Case(args=(10,), expected=3_628_800, name="ten"),
    ],
)
