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
