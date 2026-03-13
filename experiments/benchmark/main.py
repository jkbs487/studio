# Python 性能测试
# 运行: python main.py

import time
import sys

# 测试1: 斐波那契数列（纯递归，无优化）
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

# 测试2: 质数筛选
def count_primes(n):
    is_prime = [True] * (n + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(n ** 0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, n + 1, i):
                is_prime[j] = False
    return sum(is_prime)

# 测试3: 字符串拼接
def string_concat(n):
    result = ''
    for i in range(n):
        result += str(i)
    return result

# 测试4: 冒泡排序
def bubble_sort(arr):
    n = len(arr)
    for i in range(n - 1):
        for j in range(n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]

def main():
    print("========== Python 性能测试 ==========")
    print(f"Python 版本: {sys.version.split()[0]}")
    print("=====================================")

    # 测试1: 斐波那契数列（纯递归，无优化）
    start = time.time()
    fib_result = fibonacci(30)
    end = time.time()
    duration = int((end - start) * 1000)
    print(f"测试1 - 斐波那契(30): {fib_result} | 耗时: {duration} ms")

    # 测试2: 质数筛选
    start = time.time()
    prime_count = count_primes(100000)
    end = time.time()
    duration = int((end - start) * 1000)
    print(f"测试2 - 质数筛选(100000): {prime_count} 个 | 耗时: {duration} ms")

    # 测试3: 字符串拼接
    start = time.time()
    str_result = string_concat(10000)
    end = time.time()
    duration = int((end - start) * 1000)
    print(f"测试3 - 字符串拼接(10000次): {len(str_result)} 字符 | 耗时: {duration} ms")

    # 测试4: 数组排序
    arr = list(range(5000, 0, -1))
    start = time.time()
    bubble_sort(arr)
    end = time.time()
    duration = int((end - start) * 1000)
    is_correct = arr[0] == 1
    print(f"测试4 - 冒泡排序(5000元素): {is_correct} | 耗时: {duration} ms")

    print("=====================================")
    print("Python 测试完成")

if __name__ == "__main__":
    main()
