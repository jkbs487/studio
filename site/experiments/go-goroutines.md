---
title: go-goroutines
description: 探索 Go 并发编程：goroutine、channel、select、sync 包、上下文取消
---

# go-goroutines

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | go |
| 版本 | 1.18+ |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `goroutine` `channel` `concurrency` `concurrent` |

</div>

## 描述

探索 Go 并发编程：goroutine、channel、select、sync 包、上下文取消

# Go 并发编程 `Go 1.18+`

探索 Go 的并发编程特性和模式。

> **语言版本**: Go 1.18 或更高（需要泛型支持）

## 运行

```bash
go run languages/go/goroutines/main.go
```

## 内容

### 基础 Goroutine
- `go` 关键字启动协程
- 匿名函数协程

### Channel 通道
- 创建通道 `make(chan T)`
- 发送和接收 `<-`
- 有缓冲通道和无缓冲通道
- 关闭通道 `close()`

### Select 语句
- 多通道选择
- `default` 分支
- 超时处理

### 同步原语 (sync 包)
- `sync.WaitGroup` 等待组
- `sync.Mutex` 互斥锁
- `sync.RWMutex` 读写锁
- `sync.Once` 单次执行
- `sync.Cond` 条件变量

### 上下文取消 (context)
- `context.Background()` 创建根上下文
- `context.WithCancel()` 创建可取消上下文
- `context.WithTimeout()` 创建超时上下文
- `context.WithDeadline()` 创建截止时间上下文

### 原子操作 (sync/atomic)
- `atomic.AddInt64` 原子加法
- `atomic.LoadInt64` 原子读取
- `atomic.StoreInt64` 原子存储

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.go` - 实验代码


## 源码

<details>
<summary>main.go</summary>

```go
package main

import (
	"context"
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

// ============================================================
// 基础 Goroutine
// ============================================================

func basicGoroutine() {
	fmt.Println("\n--- 基础 Goroutine ---")

	// 启动匿名函数协程
	go func(msg string) {
		fmt.Println("goroutine:", msg)
	}("Hello from goroutine")

	// 启动命名函数协程
	go sayHello("World")

	// 等待一下，让协程有机会执行
	time.Sleep(100 * time.Millisecond)
}

func sayHello(name string) {
	fmt.Printf("Hello, %s!\n", name)
}

// ============================================================
// Channel 通道
// ============================================================

func channelDemo() {
	fmt.Println("\n--- Channel 通道 ---")

	// 创建无缓冲通道
	unbuffered := make(chan string)

	// 启动发送协程
	go func() {
		unbuffered <- "message from unbuffered channel"
	}()

	// 接收消息（会阻塞直到有消息）
	msg := <-unbuffered
	fmt.Println("Received:", msg)

	// 创建有缓冲通道（容量为 3）
	buffered := make(chan string, 3)

	// 发送多个消息到缓冲通道
	buffered <- "first"
	buffered <- "second"
	buffered <- "third"

	// 从缓冲通道接收
	fmt.Println("Buffered channel:")
	fmt.Println(<-buffered)
	fmt.Println(<-buffered)
	fmt.Println(<-buffered)

	// 关闭通道
	close(buffered)

	// 遍历关闭的通道
	fmt.Println("Iterating over closed channel:")
	for msg := range buffered {
		fmt.Println(msg)
	}
}

// ============================================================
// Select 语句
// ============================================================

func selectDemo() {
	fmt.Println("\n--- Select 语句 ---")

	// 创建两个通道
	ch1 := make(chan string)
	ch2 := make(chan string)

	// 启动两个协程分别向两个通道发送消息
	go func() {
		time.Sleep(50 * time.Millisecond)
		ch1 <- "from channel 1"
	}()

	go func() {
		time.Sleep(100 * time.Millisecond)
		ch2 <- "from channel 2"
	}()

	// 使用 select 等待第一个可用的通道
	for i := 0; i < 2; i++ {
		select {
		case msg1 := <-ch1:
			fmt.Println("Received:", msg1)
		case msg2 := <-ch2:
			fmt.Println("Received:", msg2)
		}
	}

	// select with default - 非阻塞接收
	fmt.Println("\nNon-blocking select:")
	msg := make(chan string)
	select {
	case m := <-msg:
		fmt.Println("Received:", m)
	default:
		fmt.Println("No message received (non-blocking)")
	}
}

// ============================================================
// WaitGroup 等待组
// ============================================================

func waitGroupDemo() {
	fmt.Println("\n--- WaitGroup 等待组 ---")

	var wg sync.WaitGroup

	// 添加 3 个任务
	wg.Add(3)

	// 启动 3 个协程
	go worker(&wg, 1)
	go worker(&wg, 2)
	go worker(&wg, 3)

	// 等待所有任务完成
	wg.Wait()
	fmt.Println("All workers completed!")
}

func worker(wg *sync.WaitGroup, id int) {
	defer wg.Done() // 完成后减少计数
	fmt.Printf("Worker %d starting\n", id)
	time.Sleep(time.Duration(id) * 100 * time.Millisecond)
	fmt.Printf("Worker %d done\n", id)
}

// ============================================================
// Mutex 互斥锁
// ============================================================

func mutexDemo() {
	fmt.Println("\n--- Mutex 互斥锁 ---")

	// 共享资源
	counter := 0
	var mu sync.Mutex

	// 启动 100 个协程，每个增加计数器
	var wg sync.WaitGroup
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			mu.Lock()
			counter++
			mu.Unlock()
		}()
	}

	wg.Wait()
	fmt.Printf("Final counter value: %d (expected: 100)\n", counter)
}

// ============================================================
// RWMutex 读写锁
// ============================================================

func rwMutexDemo() {
	fmt.Println("\n--- RWMutex 读写锁 ---")

	// 共享数据
	data := make(map[string]int)
	var rwmu sync.RWMutex
	var wg sync.WaitGroup

	// 启动多个读 goroutine
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			for j := 0; j < 10; j++ {
				rwmu.RLock()
				_ = data["key"] // 读取
				rwmu.RUnlock()
				time.Sleep(10 * time.Millisecond)
			}
		}(i)
	}

	// 启动写 goroutine
	wg.Add(1)
	go func() {
		defer wg.Done()
		for j := 0; j < 10; j++ {
			rwmu.Lock()
			data["key"] = j // 写入
			rwmu.Unlock()
			time.Sleep(10 * time.Millisecond)
		}
	}()

	wg.Wait()
	fmt.Println("RWMutex demo completed")
}

// ============================================================
// Once 单次执行
// ============================================================

func onceDemo() {
	fmt.Println("\n--- Once 单次执行 ---")

	var once sync.Once
	var wg sync.WaitGroup

	// 多个协程都尝试执行同一个函数
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			once.Do(func() {
				fmt.Println("This will only print once!")
			})
			fmt.Printf("Goroutine %d completed\n", id)
		}(i)
	}

	wg.Wait()
}

// ============================================================
// Context 上下文
// ============================================================

func contextDemo() {
	fmt.Println("\n--- Context 上下文 ---")

	// 创建可取消的上下文
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	// 启动协程
	go func() {
		select {
		case <-time.After(200 * time.Millisecond):
			fmt.Println("Operation completed normally")
		case <-ctx.Done():
			fmt.Println("Operation cancelled:", ctx.Err())
		}
	}()

	// 取消上下文
	time.Sleep(50 * time.Millisecond)
	cancel()
	time.Sleep(50 * time.Millisecond)

	// 超时上下文
	ctxTimeout, cancelTimeout := context.WithTimeout(context.Background(), 100*time.Millisecond)
	defer cancelTimeout()

	select {
	case <-ctxTimeout.Done():
		fmt.Println("Timeout context:", ctxTimeout.Err())
	}
}

// ============================================================
// 原子操作
// ============================================================

func atomicDemo() {
	fmt.Println("\n--- 原子操作 ---")

	var counter int64 = 0
	var wg sync.WaitGroup

	// 启动 100 个协程，每个使用原子操作增加计数器
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			atomic.AddInt64(&counter, 1)
		}()
	}

	wg.Wait()

	// 原子读取
	finalValue := atomic.LoadInt64(&counter)
	fmt.Printf("Atomic counter: %d (expected: 100)\n", finalValue)

	// 原子交换
	var oldValue int64
	oldValue = atomic.SwapInt64(&counter, 0)
	fmt.Printf("Swapped value: %d, new value: %d\n", oldValue, atomic.LoadInt64(&counter))
}

// ============================================================
// 生产者-消费者模式
// ============================================================

func producerConsumerDemo() {
	fmt.Println("\n--- 生产者-消费者模式 ---")

	const numProducers = 2
	const numConsumers = 3
	const itemsPerProducer = 5

	ch := make(chan int, 10)
	var producerWg sync.WaitGroup
	var consumerWg sync.WaitGroup

	// 启动生产者
	for p := 0; p < numProducers; p++ {
		producerWg.Add(1)
		go func(id int) {
			defer producerWg.Done()
			for i := 0; i < itemsPerProducer; i++ {
				item := id*itemsPerProducer + i
				ch <- item
				fmt.Printf("Producer %d produced: %d\n", id, item)
			}
		}(p)
	}

	// 启动消费者
	for c := 0; c < numConsumers; c++ {
		consumerWg.Add(1)
		go func(id int) {
			defer consumerWg.Done()
			for {
				item, ok := <-ch
				if !ok {
					return
				}
				fmt.Printf("Consumer %d consumed: %d\n", id, item)
			}
		}(c)
	}

	// 等待生产者完成
	producerWg.Wait()
	// 关闭通道，通知消费者没有更多数据
	close(ch)
	// 等待消费者完成
	consumerWg.Wait()

	fmt.Println("Producer-Consumer demo completed")
}

// ============================================================
// 主函数
// ============================================================

func main() {
	fmt.Println("Go 并发编程实验")
	fmt.Println("================")

	basicGoroutine()
	channelDemo()
	selectDemo()
	waitGroupDemo()
	mutexDemo()
	rwMutexDemo()
	onceDemo()
	contextDemo()
	atomicDemo()
	producerConsumerDemo()

	fmt.Println("\n================")
	fmt.Println("实验完成!")
}

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
