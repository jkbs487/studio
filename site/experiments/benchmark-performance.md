---
title: benchmark-performance
description: 经典性能对比实验：对比 Go、C++、Python 在计算密集型任务上的性能表现
---

# benchmark-performance

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | multi |
| 版本 | C++20, Go 1.18+, Python 3.8+ |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `benchmark` `performance` `comparison` `cpp` `go` `python` |

</div>

## 描述

经典性能对比实验：对比 Go、C++、Python 在计算密集型任务上的性能表现

# 经典性能对比实验

本实验对比 Go、C++、Python 三种语言在经典计算任务上的性能表现。

> **语言版本要求**: C++20, Go 1.18+, Python 3.8+

## 实验内容

### 测试任务

| 测试 | 描述 | 数据规模 |
|------|------|----------|
| 测试1 | 斐波那契数列（递归） | n=30 |
| 测试2 | 质数筛选（埃拉托斯特尼筛法） | n=100000 |
| 测试3 | 字符串拼接 | 10000次 |
| 测试4 | 冒泡排序 | 5000元素 |

### 测试说明

1. **斐波那契数列**: 经典的递归计算，测试函数调用开销和栈帧管理
2. **质数筛选**: 经典的筛选算法，测试循环和条件判断效率
3. **字符串拼接**: 测试字符串处理和内存分配
4. **冒泡排序**: 测试数组操作和排序算法效率

## 运行方式

### 统一脚本（推荐）

```bash
cd experiments/benchmark

# 编译并运行（默认）
./run.sh

# 仅编译
./run.sh build

# 仅运行
./run.sh run

# 清理生成文件
./run.sh clean
```

### 手动运行

**编译:**
```bash
# C++ (优化级别 -O2)
g++ -std=c++11 -O2 -o benchmark_cpp main.cpp

# Go (默认已优化，-ldflags="-s -w" 去掉符号表)
go build -ldflags="-s -w" -o benchmark_go main.go
```

**运行:**
```bash
./benchmark_cpp      # C++
./benchmark_go       # Go
python3 main.py      # Python
```

**清理:**
```bash
rm -f benchmark_cpp benchmark_go
```

## 语言特性分析

### C++

- 编译型语言，直接生成机器码
- 手动内存管理，性能最优
- 优化选项 `-O2` 开启编译器优化

### Go

- 编译型语言，性能接近 C++
- 垃圾回收带来的轻微开销
- 字符串处理使用 `strings.Builder` 优化

### Python

- 解释型语言，运行时开销最大
- 纯递归实现，无额外优化
- 解释型语言的典型表现

## 实验设计过程

### 测试任务选择

选择了四种经典的计算密集型任务，覆盖不同场景：

| 测试 | 测试重点 | 选择原因 |
|------|----------|----------|
| 斐波那契数列 | 函数调用、递归栈 | 经典递归问题，测试语言运行时的调用开销 |
| 质数筛选 | 循环、数组操作 | 经典算法，测试基本循环和内存访问效率 |
| 字符串拼接 | 内存分配、字符串处理 | 测试动态内存管理和字符串操作效率 |
| 冒泡排序 | 数组操作、比较交换 | O(n²) 复杂度，放大性能差异便于观察 |

### 遇到的问题与解决

**问题1: Python 斐波那契测试不公平**

初始实现使用了 memoization 缓存优化，导致 Python 测试结果异常快（~0.01ms），与 C++/Go 的纯递归实现不可比。

解决：去掉缓存，使用纯递归实现。同时将 n 从 40 改为 30，避免 Python 运行时间过长（fib(40) 约需 30+ 秒）。

**问题2: 编译优化级别不一致**

初始使用 `go build` 和 `g++ -O2`，优化级别可能不对等。

解决：统一优化策略：
- C++: `g++ -std=c++11 -O2`（明确的 -O2 优化）
- Go: `go build -ldflags="-s -w"`（去掉符号表，相当于 strip）

**问题3: 时间单位不统一**

初始输出中，Go 使用动态单位（ms/µs），C++ 和 Python 固定使用 ms，且 Python 有小数。

解决：统一使用整数毫秒（ms）作为输出单位，保持格式一致 `X ms`。

### 最终测试结果

macOS 环境（实际结果因硬件而异）：

| 测试 | C++ | Go | Python |
|------|-----|-----|--------|
| 斐波那契(30) | 3ms | 4ms | 132ms |
| 质数筛选(100000) | 0ms | 0ms | 3ms |
| 字符串拼接(10000) | 0ms | 1ms | 1ms |
| 冒泡排序(5000) | 7ms | 31ms | 1114ms |

### 性能结论

1. **C++ 与 Go 性能接近**：编译型语言在计算密集型任务上表现优异
2. **Python 慢 1-2 个数量级**：解释型语言的典型特征，但在 I/O 密集型场景差距会缩小
3. **冒泡排序差异最明显**：O(n²) 算法放大了语言间的性能差距
4. **字符串处理**：Go 使用 `strings.Builder` 优化后性能优异

## 注意事项

1. 结果受硬件、操作系统、运行时版本影响
2. 编译优化级别会影响 C++/Go 性能
3. 各语言均使用原生实现，无特殊优化
4. 实际项目中应根据场景选择语言

## 相关链接

- [C++ 参考文档](https://zh.cppreference.com/)
- [Go 官方文档](https://go.dev/doc/)
- [Python 官方文档](https://docs.python.org/3/)
- [CLAUDE.md](../../CLAUDE.md) - AI 协作指南


## 源码

<details>
<summary>main.cpp</summary>

```cpp
// C++ 性能测试
// 编译: g++ -O2 -o benchmark_cpp main.cpp
// 运行: ./benchmark_cpp

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace chrono;

// 测试1: 斐波那契数列（递归）
long long fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 测试2: 质数筛选
int countPrimes(int n) {
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    int count = 0;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) count++;
    }
    return count;
}

// 测试3: 字符串拼接
string stringConcat(int n) {
    string result;
    for (int i = 0; i < n; i++) {
        result += to_string(i);
    }
    return result;
}

// 测试4: 数组排序
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    cout << "========== C++ 性能测试 ==========" << endl;
    cout << "编译优化: -O2" << endl;
    cout << "=================================" << endl;

    // 测试1: 斐波那契数列
    auto start = high_resolution_clock::now();
    long long fibResult = fibonacci(30);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "测试1 - 斐波那契(30): " << fibResult << " | 耗时: " << duration.count() << " ms" << endl;

    // 测试2: 质数筛选
    start = high_resolution_clock::now();
    int primeCount = countPrimes(100000);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "测试2 - 质数筛选(100000): " << primeCount << " 个 | 耗时: " << duration.count() << " ms" << endl;

    // 测试3: 字符串拼接
    start = high_resolution_clock::now();
    string strResult = stringConcat(10000);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "测试3 - 字符串拼接(10000次): " << strResult.length() << " 字符 | 耗时: " << duration.count() << " ms" << endl;

    // 测试4: 数组排序
    vector<int> arr(5000);
    for (int i = 0; i < 5000; i++) {
        arr[i] = 5000 - i;
    }
    start = high_resolution_clock::now();
    bubbleSort(arr);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "测试4 - 冒泡排序(5000元素): " << (arr[0] == 1 ? "正确" : "错误") << " | 耗时: " << duration.count() << " ms" << endl;

    cout << "=================================" << endl;
    cout << "C++ 测试完成" << endl;

    return 0;
}

```

</details>

<details>
<summary>main.go</summary>

```go
package main

import (
	"fmt"
	"strings"
	"time"
)

// 测试1: 斐波那契数列（递归）
func fibonacci(n int) int {
	if n <= 1 {
		return n
	}
	return fibonacci(n-1) + fibonacci(n-2)
}

// 测试2: 质数筛选
func countPrimes(n int) int {
	isPrime := make([]bool, n+1)
	for i := 2; i <= n; i++ {
		isPrime[i] = true
	}
	for i := 2; i*i <= n; i++ {
		if isPrime[i] {
			for j := i * i; j <= n; j += i {
				isPrime[j] = false
			}
		}
	}
	count := 0
	for i := 2; i <= n; i++ {
		if isPrime[i] {
			count++
		}
	}
	return count
}

// 测试3: 字符串拼接
func stringConcat(n int) string {
	var builder strings.Builder
	for i := 0; i < n; i++ {
		builder.WriteString(fmt.Sprintf("%d", i))
	}
	return builder.String()
}

// 测试4: 冒泡排序
func bubbleSort(arr []int) {
	n := len(arr)
	for i := 0; i < n-1; i++ {
		for j := 0; j < n-i-1; j++ {
			if arr[j] > arr[j+1] {
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}
		}
	}
}

func main() {
	fmt.Println("========== Go 性能测试 ==========")
	fmt.Println("编译优化: go build -ldflags=\"-s -w\"")
	fmt.Println("==================================")

	// 测试1: 斐波那契数列
	start := time.Now()
	fibResult := fibonacci(30)
	end := time.Now()
	duration := end.Sub(start).Milliseconds()
	fmt.Printf("测试1 - 斐波那契(30): %d | 耗时: %d ms\n", fibResult, duration)

	// 测试2: 质数筛选
	start = time.Now()
	primeCount := countPrimes(100000)
	end = time.Now()
	duration = end.Sub(start).Milliseconds()
	fmt.Printf("测试2 - 质数筛选(100000): %d 个 | 耗时: %d ms\n", primeCount, duration)

	// 测试3: 字符串拼接
	start = time.Now()
	strResult := stringConcat(10000)
	end = time.Now()
	duration = end.Sub(start).Milliseconds()
	fmt.Printf("测试3 - 字符串拼接(10000次): %d 字符 | 耗时: %d ms\n", len(strResult), duration)

	// 测试4: 数组排序
	arr := make([]int, 5000)
	for i := 0; i < 5000; i++ {
		arr[i] = 5000 - i
	}
	start = time.Now()
	bubbleSort(arr)
	end = time.Now()
	duration = end.Sub(start).Milliseconds()
	isCorrect := arr[0] == 1
	fmt.Printf("测试4 - 冒泡排序(5000元素): %v | 耗时: %d ms\n", isCorrect, duration)

	fmt.Println("==================================")
	fmt.Println("Go 测试完成")
}

```

</details>

<details>
<summary>main.py</summary>

```python
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
