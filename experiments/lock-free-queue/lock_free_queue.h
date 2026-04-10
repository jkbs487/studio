#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include <optional>
#include <queue>
#include <mutex>
#include <functional>

// ============================================================================
// Lock-Free Stack - 使用 Tagged Pointer 解决 ABA 问题
// ============================================================================
template<typename T>
struct LockFreeStackNode {
    T data;
    std::atomic<LockFreeStackNode*> next;

    explicit LockFreeStackNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class LockFreeStack {
public:
    LockFreeStack() : head(0) {}

    void push(T value) {
        auto* newNode = new LockFreeStackNode<T>(std::move(value));

        while (true) {
            uintptr_t oldHead = head.load(std::memory_order_seq_cst);

            LockFreeStackNode<T>* oldPtr = reinterpret_cast<LockFreeStackNode<T>*>(oldHead & 0xFFFFFFFFFFFF);
            uint64_t oldTag = oldHead >> 48;

            newNode->next.store(oldPtr, std::memory_order_seq_cst);

            uintptr_t newHead = reinterpret_cast<uintptr_t>(newNode) | ((oldTag + 1) << 48);

            if (head.compare_exchange_weak(oldHead, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                return;
            }
        }
    }

    std::optional<T> pop() {
        while (true) {
            uintptr_t oldHead = head.load(std::memory_order_seq_cst);

            LockFreeStackNode<T>* ptr = reinterpret_cast<LockFreeStackNode<T>*>(oldHead & 0xFFFFFFFFFFFF);
            uint64_t oldTag = oldHead >> 48;

            if (ptr == nullptr) {
                return std::nullopt;
            }

            LockFreeStackNode<T>* nextNode = ptr->next.load(std::memory_order_seq_cst);

            uintptr_t newHead = reinterpret_cast<uintptr_t>(nextNode) | ((oldTag + 1) << 48);

            if (head.compare_exchange_weak(oldHead, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                T value = std::move(ptr->data);
                delete ptr;
                return value;
            }
        }
    }

    bool isEmpty() const {
        return (head.load(std::memory_order_seq_cst) & 0xFFFFFFFFFFFF) == 0;
    }

    ~LockFreeStack() {
        while (pop()) {}
    }

private:
    std::atomic<uintptr_t> head;
};

// ============================================================================
// Lock-Free Queue - Michael-Scott Queue
// ============================================================================
template<typename T>
struct LockFreeQueueNode {
    T data;
    std::atomic<LockFreeQueueNode*> next;

    explicit LockFreeQueueNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class LockFreeQueue {
public:
    LockFreeQueue() {
        auto* dummy = new LockFreeQueueNode<T>(T{});
        dummy->next.store(nullptr, std::memory_order_seq_cst);
        head.store(reinterpret_cast<uintptr_t>(dummy), std::memory_order_seq_cst);
        tail.store(reinterpret_cast<uintptr_t>(dummy), std::memory_order_seq_cst);
    }

    void enqueue(T value) {
        auto* newNode = new LockFreeQueueNode<T>(std::move(value));
        newNode->next.store(nullptr, std::memory_order_seq_cst);

        while (true) {
            uintptr_t curTail = tail.load(std::memory_order_seq_cst);
            LockFreeQueueNode<T>* tailPtr = reinterpret_cast<LockFreeQueueNode<T>*>(curTail & 0xFFFFFFFFFFFF);

            // 读取 tail->next
            LockFreeQueueNode<T>* next = tailPtr->next.load(std::memory_order_seq_cst);

            if (next != nullptr) {
                // tail 落后，帮它前进
                tail.store(reinterpret_cast<uintptr_t>(next), std::memory_order_seq_cst);
                continue;
            }

            // 尝试 CAS - 使用 strong 版本避免伪失败
            LockFreeQueueNode<T>* expected = nullptr;
            if (tailPtr->next.compare_exchange_strong(expected, newNode,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                tail.store(reinterpret_cast<uintptr_t>(newNode), std::memory_order_seq_cst);
                return;
            }
            // CAS 失败another thread succeeded，重试
        }
    }

    std::optional<T> dequeue() {
        while (true) {
            uintptr_t oldHead = head.load(std::memory_order_seq_cst);
            LockFreeQueueNode<T>* headPtr = reinterpret_cast<LockFreeQueueNode<T>*>(oldHead & 0xFFFFFFFFFFFF);
            uint64_t oldTag = oldHead >> 48;

            LockFreeQueueNode<T>* next = headPtr->next.load(std::memory_order_seq_cst);

            if (next == nullptr) {
                return std::nullopt;
            }

            uintptr_t newHead = reinterpret_cast<uintptr_t>(next) | ((oldTag + 1) << 48);
            if (head.compare_exchange_weak(oldHead, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                T value = std::move(next->data);
                delete headPtr;
                return value;
            }
        }
    }

    bool isEmpty() const {
        uintptr_t h = head.load(std::memory_order_seq_cst);
        LockFreeQueueNode<T>* headPtr = reinterpret_cast<LockFreeQueueNode<T>*>(h & 0xFFFFFFFFFFFF);
        return headPtr->next.load(std::memory_order_seq_cst) == nullptr;
    }

    ~LockFreeQueue() {
        while (true) {
            uintptr_t h = head.load(std::memory_order_seq_cst);
            LockFreeQueueNode<T>* headPtr = reinterpret_cast<LockFreeQueueNode<T>*>(h & 0xFFFFFFFFFFFF);
            LockFreeQueueNode<T>* next = headPtr->next.load(std::memory_order_seq_cst);

            if (next == nullptr) {
                delete headPtr;
                break;
            }

            uintptr_t newHead = reinterpret_cast<uintptr_t>(next);
            if (head.compare_exchange_weak(h, newHead,
                    std::memory_order_seq_cst, std::memory_order_seq_cst)) {
                delete headPtr;
            }
        }
    }

private:
    std::atomic<uintptr_t> head;
    std::atomic<uintptr_t> tail;
};

// ============================================================================
// Mutex Queue - 有锁队列对比
// ============================================================================
template<typename T>
class MutexQueue {
public:
    void enqueue(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(value));
    }

    std::optional<T> dequeue() {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return std::nullopt;
        }
        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
};

// ============================================================================
// Work Queue - 工作队列应用
// ============================================================================
class WorkQueue {
public:
    struct WorkItem {
        int id;
        std::function<void()> task;
    };

    void submit(int id, std::function<void()> task) {
        auto* item = new WorkItem{id, std::move(task)};
        queue.enqueue(item);
    }

    WorkItem* waitForWork() {
        auto result = queue.dequeue();
        if (result) {
            return *result;
        }
        return nullptr;
    }

private:
    LockFreeQueue<WorkItem*> queue;
};

#endif // LOCK_FREE_QUEUE_H