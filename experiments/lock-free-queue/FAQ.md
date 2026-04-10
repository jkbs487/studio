# 无锁队列 FAQ

## 实现经验

### Q: 为什么使用 memory_order_seq_cst？

A: `seq_cst` 是最安全的默认选择。理论上可优化为 acquire/release，但需要：
- 形式化验证确保无竞态
- 使用 Hazard Pointer 防止 use-after-free
- 两个独立 load 之间没有同步保证，容易出现 bug

正确性优先，性能其次。

### Q: 为什么并发测试时崩溃？

A: ABA 问题导致。多个线程并发 push/pop 时：
1. 线程A pop 节点 X，删除它
2. 线程B push 新节点，操作系统恰好复用同一地址
3. 线程A 的 CAS 认为 X 没变，成功执行，导致 double-free

解决方案：Tagged Pointer（将指针和版本号打包到 64 位）。

### Q: 纯入队为什么比有锁慢？

A: Michael-Scott 队列的设计限制：
- 每个 enqueue 需要检查 tail 是否落后
- 队列长时，tail 经常落后
- 反复检测开销大于锁竞争

这是算法特性，不是实现 bug。

### Q: 正确的性能测试姿势？

A: 必须使用生产者-消费者模式：
- 多个生产者 enqueue
- 多个消费者 dequeue
- 平衡生产和消费，避免队列过长

混合 enqueue/dequeue 不是该算法的设计用途。

### Q: 如何选择有锁还是无锁？

A: 根据场景：

| 场景 | 推荐 |
|------|------|
| 高并发 + 生产者-消费者 | 无锁 |
| 低并发 + 单一操作 | 有锁 |
| 队列很长 + 纯入队 | 有锁 |
| 延迟敏感 | 无锁 |

### Q: 为什么 LockFreeStack 比 LockFreeQueue 简单？

A: Stack 只有一个操作点（head），Queue 有两个（head 和 tail）。
Queue 需要协调两端，tail 可能落后，需要额外帮助推进。

### Q: 实验的关键结论？

A:
- 有消费者时：无锁始终更快（避免锁争用）
- 纯入队时：有锁略快（Michael-Scott 算法限制）
- 高并发 + 生产者消费者：无锁最佳场景