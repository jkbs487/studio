# C++ 无锁队列 `C++20`

探索 C++ 无锁队列（Lock-Free Queue）的实现原理和实践应用。

> **语言版本**: C++20

## 包含内容

1. **原子操作基础** - std::atomic、内存序（memory order）
2. **CAS（Compare-And-Swap）** - 比较并交换操作
3. **无锁栈实现** - 基于 CAS 的栈数据结构
4. **无锁队列实现** - Michael-Scott 队列
5. **内存模型** - 内存序对并发正确性的影响
6. **ABA 问题** - Tagged Pointer 解决方案
7. **性能对比** - 无锁 vs 有锁队列的多场景测试

## 核心概念

### 无锁栈 (LockFreeStack)

基于单链表的头插法栈，使用 CAS 实现 push/pop。

### 无锁队列 (LockFreeQueue)

Michael-Scott 队列算法，使用哨兵节点简化边界处理。

###  Tagged Pointer

将指针和版本号打包到 64 位，解决 ABA 问题。

## 编译与运行

**手动编译：**
```bash
cd experiments/lock-free-queue
mkdir build && cd build
cmake .. && make
./lock-free-queue
```

## 性能测试

运行程序查看不同场景下的性能对比：

- 单生产者+消费者
- 多生产者+消费者
- 纯入队场景
- 高并发场景

详见 `./lock-free-queue` 输出的测试结果。

## 实现经验 FAQ

常见问题和解答见 [FAQ.md](./FAQ.md)。

## 最佳实践

- 简单场景优先考虑有锁结构
- 无锁仅在高并发、延迟敏感场景使用
- 先 profiling 再优化

## 要求

- C++20 或更高版本
- 线程支持库