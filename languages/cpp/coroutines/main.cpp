#include <coroutine>
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <chrono>
#include <optional>
#include <stdexcept>

using namespace std::chrono_literals;

// ============================================================
// 基础协程概念
// ============================================================

// suspend_always: 总是暂停协程
struct suspend_always {
    constexpr bool await_ready() const noexcept { return false; }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

// suspend_never: 从不暂停协程（立即执行）
struct suspend_never {
    constexpr bool await_ready() const noexcept { return true; }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

// ============================================================
// 简单的 Task (协程返回对象)
// ============================================================

struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// ============================================================
// Generator (生成器) - 使用 co_yield
// ============================================================

template<typename T>
struct Generator {
    struct promise_type {
        T value;
        std::exception_ptr exception;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T val) {
            value = val;
            return {};
        }
        void return_void() {}
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> handle;

    Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    // Range-based for loop 支持
    bool next() {
        if (!handle) return false;
        if (handle.done()) return false;
        handle.resume();
        if (handle.done()) {
            // 协程已完成，检查是否有异常
            if (handle.promise().exception) {
                std::rethrow_exception(handle.promise().exception);
            }
            return false;
        }
        return true;
    }

    T& current() { return handle.promise().value; }
};

// ============================================================
// 演示1: 基础协程
// ============================================================

void basicCoroutine() {
    std::cout << "\n--- 基础协程 ---" << std::endl;

    // 简单的协程函数
    auto simpleCoroutine = []() -> Task {
        std::cout << "协程开始执行" << std::endl;
        co_await std::suspend_always{};  // 暂停
        std::cout << "协程恢复执行" << std::endl;
        co_return;
    };

    auto task = simpleCoroutine();
    std::cout << "协程已创建（暂停状态）" << std::endl;
    // Task 是立即暂停的，不需要手动恢复
    std::cout << "基础协程演示完成" << std::endl;
}

// ============================================================
// 演示2: 生成器 - co_yield
// ============================================================

Generator<int> countTo(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i;  // 产生值并暂停
    }
}

void generatorDemo() {
    std::cout << "\n--- 生成器 (co_yield) ---" << std::endl;

    // 创建生成器
    auto gen = countTo(5);

    std::cout << "生成 1-5:" << std::endl;
    while (gen.next()) {
        std::cout << gen.current() << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示3: 字符串生成器
// ============================================================

Generator<std::string> fibonacciStrings(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield "fib(" + std::to_string(i + 1) + ")=" + std::to_string(a);
        int next = a + b;
        a = b;
        b = next;
    }
}

void stringGeneratorDemo() {
    std::cout << "\n--- 字符串生成器 ---" << std::endl;

    auto gen = fibonacciStrings(8);
    while (gen.next()) {
        std::cout << gen.current() << ", ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示4: co_await 与自定义 awaitable
// ============================================================

// 同步等待的 awaitable
struct SyncSleepAwaitable {
    std::chrono::milliseconds duration;

    explicit SyncSleepAwaitable(std::chrono::milliseconds d) : duration(d) {}

    bool await_ready() const { return false; }

    void await_suspend(std::coroutine_handle<> h) const {
        std::this_thread::sleep_for(duration);
        h.resume();  // 手动恢复协程
    }

    void await_resume() const {}
};

// 立即执行的 Task (使用 suspend_never)
struct ImmediateTask {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        ImmediateTask get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return ImmediateTask{h};
        }
        std::suspend_never initial_suspend() { return {}; }  // 立即开始
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    ImmediateTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~ImmediateTask() { if (handle) handle.destroy(); }
    ImmediateTask(const ImmediateTask&) = delete;
    ImmediateTask& operator=(const ImmediateTask&) = delete;
    ImmediateTask(ImmediateTask&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    ImmediateTask& operator=(ImmediateTask&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }
};

ImmediateTask syncAsyncTask() {
    std::cout << "开始同步异步任务 (将等待 50ms)..." << std::endl;
    co_await SyncSleepAwaitable(50ms);
    std::cout << "同步异步任务完成!" << std::endl;
}

void coAwaitDemo() {
    std::cout << "\n--- co_await 自定义 awaitable ---" << std::endl;
    auto task = syncAsyncTask();
    // Task 会在这里析构，自动清理协程
}

// ============================================================
// 演示5: 协程返回值
// ============================================================

struct SimpleFuture {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        int value = 0;
        SimpleFuture get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return SimpleFuture{h};
        }
        std::suspend_never initial_suspend() { return {}; }  // 立即执行
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { value = v; }
        void unhandled_exception() { std::terminate(); }
    };

    int value() const { return handle.promise().value; }
};

SimpleFuture asyncAdd(int a, int b) {
    co_return a + b;
}

void futureCoroutineDemo() {
    std::cout << "\n--- 协程返回值 ---" << std::endl;

    auto fut = asyncAdd(10, 20);
    // 使用 suspend_never，协程立即执行，return_value 已被调用
    // 通过 handle 获取 promise 中的值
    std::cout << "10 + 20 = " << fut.value() << std::endl;
    fut.handle.destroy();  // 清理
}

// ============================================================
// 演示6: 无限生成器
// ============================================================

Generator<int> infiniteCounter() {
    int i = 0;
    while (true) {
        co_yield i;
        ++i;
    }
}

void infiniteGeneratorDemo() {
    std::cout << "\n--- 无限生成器 ---" << std::endl;

    auto counter = infiniteCounter();
    for (int i = 0; i < 10; ++i) {
        counter.next();
        std::cout << counter.current() << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示7: 使用 std::optional 的生成器
// ============================================================

template<typename T>
struct OptionalGenerator {
    struct promise_type {
        std::optional<T> value;
        std::exception_ptr exception;

        OptionalGenerator get_return_object() {
            return OptionalGenerator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T val) {
            value = val;
            return {};
        }
        void return_void() {}
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> handle;

    OptionalGenerator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~OptionalGenerator() { if (handle) handle.destroy(); }
    OptionalGenerator(OptionalGenerator&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    OptionalGenerator& operator=(OptionalGenerator&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    std::optional<T> next() {
        if (!handle || handle.done()) return std::nullopt;
        handle.resume();
        if (handle.done()) return std::nullopt;
        auto val = handle.promise().value;
        return val;
    }
};

OptionalGenerator<int> filterEven(int n) {
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            co_yield i;
        }
    }
}

void optionalGeneratorDemo() {
    std::cout << "\n--- 使用 std::optional 的生成器 ---" << std::endl;

    auto gen = filterEven(10);
    while (auto val = gen.next()) {
        std::cout << *val << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示8: 协程链 - 组合多个协程
// ============================================================

Generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i) {
        co_yield i;
    }
}

Generator<int> doubleValues(Generator<int> input) {
    while (input.next()) {
        co_yield input.current() * 2;
    }
}

void coroutineChainDemo() {
    std::cout << "\n--- 协程链 ---" << std::endl;

    std::cout << "range(1, 6): ";
    auto r = range(1, 6);
    while (r.next()) {
        std::cout << r.current() << " ";
    }
    std::cout << std::endl;

    std::cout << "doubleValues(range(1, 6)): ";
    auto doubled = doubleValues(range(1, 6));
    while (doubled.next()) {
        std::cout << doubled.current() << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 演示9: lazy 初始化协程（惰性执行）
// ============================================================

struct LazyTask {
    struct promise_type;
    std::coroutine_handle<promise_type> handle;

    struct promise_type {
        LazyTask get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            return LazyTask{h};
        }
        std::suspend_always initial_suspend() { return {}; }  // 先暂停，不立即执行
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    LazyTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~LazyTask() { if (handle) handle.destroy(); }
    LazyTask(const LazyTask&) = delete;
    LazyTask& operator=(const LazyTask&) = delete;
    LazyTask(LazyTask&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    LazyTask& operator=(LazyTask&& other) noexcept {
        if (this != &other) {
            if (handle) handle.destroy();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    // 手动恢复执行
    void resume() {
        if (handle && !handle.done()) {
            handle.resume();
        }
    }

    bool done() const { return !handle || handle.done(); }
};

LazyTask lazyCoroutine() {
    std::cout << "lazy 协程: 开始执行" << std::endl;
    std::cout << "lazy 协程: 结束执行" << std::endl;
    co_return;
}

void lazyCoroutineDemo() {
    std::cout << "\n--- Lazy 初始化协程（惰性执行） ---" << std::endl;
    
    std::cout << "1. 创建协程任务..." << std::endl;
    auto task = lazyCoroutine();
    
    std::cout << "2. 协程已创建，但尚未执行（因为是 lazy）" << std::endl;
    
    std::cout << "3. 手动调用 resume()..." << std::endl;
    task.resume();
    
    std::cout << "4. 协程执行完毕" << std::endl;
}

// ============================================================
// 演示10: 异常处理
// ============================================================

Generator<int> exceptionGenerator(bool shouldThrow) {
    co_yield 1;
    co_yield 2;
    if (shouldThrow) {
        throw std::runtime_error("协程中的异常");
    }
    co_yield 3;
}

void exceptionDemo() {
    std::cout << "\n--- 协程异常处理 ---" << std::endl;

    // 不抛异常的生成器
    std::cout << "正常生成器: ";
    auto gen1 = exceptionGenerator(false);
    while (gen1.next()) {
        std::cout << gen1.current() << " ";
    }
    std::cout << std::endl;

    // 抛异常的生成器
    std::cout << "异常生成器: ";
    auto gen2 = exceptionGenerator(true);
    try {
        while (gen2.next()) {
            std::cout << gen2.current() << " ";
        }
    } catch (...) {
        std::cout << "捕获到异常!" << std::endl;
    }
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "C++20 协程实验" << std::endl;
    std::cout << "==============" << std::endl;

    basicCoroutine();
    generatorDemo();
    stringGeneratorDemo();
    coAwaitDemo();
    futureCoroutineDemo();
    infiniteGeneratorDemo();
    optionalGeneratorDemo();
    coroutineChainDemo();
    lazyCoroutineDemo();
    exceptionDemo();

    std::cout << "\n==============" << std::endl;
    std::cout << "实验完成!" << std::endl;
    return 0;
}
