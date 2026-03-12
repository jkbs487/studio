#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

// ============================================
// 示例类：用于演示智能指针
// ============================================

class Resource {
private:
    std::string name;
    int value;

public:
    explicit Resource(const std::string& n, int v = 0)
        : name(n), value(v) {
        std::cout << "[构造] " << name << " (value=" << value << ")" << std::endl;
    }

    ~Resource() {
        std::cout << "[析构] " << name << " (value=" << value << ")" << std::endl;
    }

    void doSomething() const {
        std::cout << "[操作] " << name << " 正在工作，值: " << value << std::endl;
    }

    void setValue(int v) {
        value = v;
    }

    int getValue() const {
        return value;
    }

    std::string getName() const {
        return name;
    }
};

// ============================================
// 1. std::unique_ptr - 独占所有权
// ============================================

void demo_unique_ptr() {
    std::cout << "\n--- 1. std::unique_ptr ---\n" << std::endl;

    // 创建 unique_ptr
    std::unique_ptr<Resource> res1 = std::make_unique<Resource>("Resource-1", 100);
    res1->doSomething();

    // 无法复制，只能移动
    std::unique_ptr<Resource> res2 = std::move(res1);
    std::cout << "移动后 res2: ";
    res2->doSomething();

    // res1 现在为空
    if (!res1) {
        std::cout << "res1 已移动，现在为空" << std::endl;
    }

    // 自定义删除器
    auto custom_deleter = [](Resource* p) {
        std::cout << "[自定义删除器] 正在删除 " << p->getName() << std::endl;
        delete p;
    };

    std::unique_ptr<Resource, decltype(custom_deleter)> res3(
        new Resource("Resource-3", 300), custom_deleter);

    // unique_ptr 与数组
    // 注意: make_unique<Resource[]>要求Resource有默认构造函数
    // 这里使用vector作为替代方案
    std::vector<std::unique_ptr<Resource>> arr;
    arr.push_back(std::make_unique<Resource>("Array-1", 101));
    arr.push_back(std::make_unique<Resource>("Array-2", 102));
    arr.push_back(std::make_unique<Resource>("Array-3", 103));
    std::cout << "创建动态数组 (使用vector<unique_ptr>): " << arr.size() << " 个元素" << std::endl;
}

// ============================================
// 2. std::shared_ptr - 共享所有权
// ============================================

void demo_shared_ptr() {
    std::cout << "\n--- 2. std::shared_ptr ---\n" << std::endl;

    // 创建 shared_ptr
    std::shared_ptr<Resource> res1 = std::make_shared<Resource>("Shared-1", 200);
    std::cout << "res1 引用计数: " << res1.use_count() << std::endl;

    // 复制 shared_ptr
    std::shared_ptr<Resource> res2 = res1;
    std::cout << "复制后 res1 引用计数: " << res1.use_count() << std::endl;
    std::cout << "res2 引用计数: " << res2.use_count() << std::endl;

    // 通过 shared_ptr 访问
    res1->doSomething();
    res2->doSomething();

    // 重置一个 shared_ptr
    res2.reset();
    std::cout << "res2.reset() 后 res1 引用计数: " << res1.use_count() << std::endl;

    // 在容器中使用 shared_ptr
    std::vector<std::shared_ptr<Resource>> vec;
    vec.push_back(res1);
    vec.push_back(std::make_shared<Resource>("Shared-2", 250));
    std::cout << "添加到 vector 后 res1 引用计数: " << res1.use_count() << std::endl;
}

// ============================================
// 3. std::weak_ptr - 弱引用
// ============================================

void demo_weak_ptr() {
    std::cout << "\n--- 3. std::weak_ptr ---\n" << std::endl;

    auto shared_res = std::make_shared<Resource>("Shared-3", 300);
    std::cout << "初始引用计数: " << shared_res.use_count() << std::endl;

    // 创建 weak_ptr，不增加引用计数
    std::weak_ptr<Resource> weak_res = shared_res;
    std::cout << "创建 weak_ptr 后引用计数: " << shared_res.use_count() << std::endl;

    // 检查 weak_ptr 是否有效
    if (auto locked = weak_res.lock()) {
        std::cout << "weak_ptr 有效，访问对象: ";
        locked->doSomething();
    }

    // 重置 shared_ptr
    shared_res.reset();
    std::cout << "shared_res.reset() 后引用计数: " << shared_res.use_count() << std::endl;

    // 再次尝试 lock
    if (auto locked = weak_res.lock()) {
        std::cout << "weak_ptr 仍有效" << std::endl;
    } else {
        std::cout << "weak_ptr 已失效（对象已被销毁）" << std::endl;
    }
}

// ============================================
// 4. 循环引用问题
// ============================================

class Node : public std::enable_shared_from_this<Node> {
private:
    std::string name;
    std::shared_ptr<Node> next;  // 使用 shared_ptr 会导致循环引用
    std::weak_ptr<Node> parent;   // 使用 weak_ptr 避免循环引用

public:
    explicit Node(const std::string& n) : name(n) {
        std::cout << "[Node构造] " << name << std::endl;
    }

    ~Node() {
        std::cout << "[Node析构] " << name << std::endl;
    }

    void setNext(const std::shared_ptr<Node>& n) {
        next = n;
        if (n) {
            n->parent = shared_from_this();  // 需要 enable_shared_from_this
        }
    }

    void display() const {
        std::cout << "Node: " << name;
        if (next) {
            std::cout << " -> " << next->name;
        }
        std::cout << std::endl;
    }

    std::string getName() const { return name; }
};

void demo_circular_reference() {
    std::cout << "\n--- 4. 循环引用演示 ---\n" << std::endl;

    // 使用 weak_ptr 可以避免循环引用
    auto node1 = std::make_shared<Node>("A");
    auto node2 = std::make_shared<Node>("B");

    node1->setNext(node2);
    node2->setNext(node1);

    std::cout << "\n链接关系:" << std::endl;
    node1->display();

    // 注意：如果没有使用 weak_ptr，这里会导致内存泄漏
    // 因为两个节点互相持有 shared_ptr，引用计数永远不会为0

    std::cout << "\nnode1 引用计数: " << node1.use_count() << std::endl;
    std::cout << "node2 引用计数: " << node2.use_count() << std::endl;

    // 由于使用了 weak_ptr，对象可以正常销毁
    std::cout << "\n离开作用域，观察析构顺序..." << std::endl;
}

// ============================================
// 5. 工厂函数最佳实践
// ============================================

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_custom(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 简单的对象池示例
class ObjectPool {
private:
    std::vector<std::unique_ptr<Resource>> pool;

public:
    void add(const std::string& name, int value) {
        pool.push_back(std::make_unique<Resource>(name, value));
    }

    Resource* get(size_t index) {
        if (index < pool.size()) {
            return pool[index].get();
        }
        return nullptr;
    }

    size_t size() const {
        return pool.size();
    }
};

void demo_best_practices() {
    std::cout << "\n--- 5. 最佳实践 ---\n" << std::endl;

    // 使用 make_shared 优化内存分配
    auto res = std::make_shared<Resource>("Optimized", 999);
    res->doSomething();

    // 在对象池中使用 unique_ptr
    ObjectPool pool;
    pool.add("Pool-1", 1);
    pool.add("Pool-2", 2);
    pool.add("Pool-3", 3);

    std::cout << "\n对象池大小: " << pool.size() << std::endl;
    if (auto obj = pool.get(1)) {
        obj->doSomething();
    }

    // 避免使用裸指针管理动态内存
    std::cout << "\n不推荐的写法（但展示对比）:" << std::endl;
    Resource* raw_ptr = new Resource("Raw-Ptr", 0);
    raw_ptr->doSomething();
    delete raw_ptr;  // 必须手动 delete，容易忘记
}

// ============================================
// 6. 异常安全示例
// ============================================

void riskyFunction(std::shared_ptr<Resource> res) {
    std::cout << "在函数中使用 shared_ptr" << std::endl;
    res->doSomething();
    // 即使抛出异常，资源也会被正确释放
    throw std::runtime_error("模拟异常");
}

void demo_exception_safety() {
    std::cout << "\n--- 6. 异常安全 ---\n" << std::endl;

    auto res = std::make_shared<Resource>("Exception-Safe", 500);

    try {
        riskyFunction(res);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }

    // res 仍然有效，资源未被泄露
    std::cout << "异常后资源仍然可用: ";
    res->doSomething();
}

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ 智能指针实验" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. unique_ptr
    demo_unique_ptr();

    // 2. shared_ptr
    demo_shared_ptr();

    // 3. weak_ptr
    demo_weak_ptr();

    // 4. 循环引用
    demo_circular_reference();

    // 5. 最佳实践
    demo_best_practices();

    // 6. 异常安全
    demo_exception_safety();

    std::cout << "\n========================================" << std::endl;
    std::cout << "实验完成!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
