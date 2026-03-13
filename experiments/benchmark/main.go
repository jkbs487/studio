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
