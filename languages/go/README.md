# Go 语言特性研究

本目录包含 Go 语言特性的实验和研究。

> **版本说明**: 各实验根据实际使用的特性标注最低版本要求，详见各实验列表

## 实验列表

### 并发编程 (goroutines) `Go 1.18+`

探索 Go 的并发编程模型。

**内容：**
- Goroutine 基础
- Channel 通道
- Select 语句
- sync 包（WaitGroup、Mutex、RWMutex、Once）
- context 上下文取消
- 原子操作

**运行：**
```bash
go run languages/go/goroutines/main.go
```

---

## 快速开始

选择感兴趣的实验目录，运行对应的 `main.go` 文件：

```bash
# 进入实验目录
cd languages/go/{experiment-name}

# 运行实验
go run main.go
```

## 实验规范

每个实验目录包含：
- `meta.json` - 实验元数据
- `README.md` - 实验说明文档
- `main.go` - 可运行的实验代码

## 查看所有实验

使用 `tools/list_experiments.py` 可以快速浏览所有实验：

```bash
python tools/list_experiments.py
```

输出示例：
```
已有实验 (N 个):

======================================

名称: go-goroutines
语言: go (1.18+)
描述: 探索 Go 并发编程：goroutine、channel、select、sync 包、上下文取消
标签: [goroutine, channel, concurrency, concurrent]
创建时间: 2026-03-13
来源: AI生成
AI参与: 完全AI
路径: languages/go/goroutines
```

## 相关链接

- [Go 官方文档](https://go.dev/doc/)
- [Go 语言规范](https://go.dev/ref/spec)
- [CLAUDE.md](../../CLAUDE.md) - AI 协作指南
