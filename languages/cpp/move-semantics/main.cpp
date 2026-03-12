#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <memory>

// ============================================
// 示例类：用于演示移动语义
// ============================================

class BigData {
private:
    std::string name;
    int* data;
    size_t size;
    bool silent;  // 静默模式标志

public:
    // 默认构造
    BigData() : data(nullptr), size(0), silent(false) {
        if (!silent) std::cout << "[默认构造] " << name << std::endl;
    }

    // 带参数构造
    BigData(const std::string& n, size_t s, bool silent_mode = false)
        : name(n), size(s), silent(silent_mode) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }
        if (!silent) std::cout << "[构造] " << name << " (大小: " << size << ")" << std::endl;
    }

    // 拷贝构造 - 深拷贝
    BigData(const BigData& other)
        : name(other.name + " (拷贝)"), size(other.size), silent(other.silent) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        if (!silent) std::cout << "[拷贝构造] " << name << std::endl;
    }

    // 移动构造 - 转移资源
    BigData(BigData&& other) noexcept
        : name(std::move(other.name) + " (移动)"),
          data(other.data),
          size(other.size),
          silent(other.silent) {
        other.data = nullptr;
        other.size = 0;
        other.name.clear();
        if (!silent) std::cout << "[移动构造] " << name << std::endl;
    }

    // 拷贝赋值
    BigData& operator=(const BigData& other) {
        if (!silent) std::cout << "[拷贝赋值] " << other.name << std::endl;
        if (this != &other) {
            delete[] data;
            name = other.name + " (赋值)";
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // 移动赋值
    BigData& operator=(BigData&& other) noexcept {
        if (!silent) std::cout << "[移动赋值] " << other.name << std::endl;
        if (this != &other) {
            delete[] data;
            name = std::move(other.name) + " (移动赋值)";
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            other.name.clear();
        }
        return *this;
    }

    // 析构
    ~BigData() {
        delete[] data;
        if (!silent && !name.empty()) {
            std::cout << "[析构] " << name << std::endl;
        }
    }

    void display() const {
        std::cout << name << " ";
        if (data) {
            std::cout << "(大小: " << size << ", 数据: [";
            size_t display_count = std::min(size, size_t(5));
            for (size_t i = 0; i < display_count; ++i) {
                std::cout << data[i];
                if (i < display_count - 1) std::cout << ", ";
            }
            if (size > 5) std::cout << "...";
            std::cout << "])";
        } else {
            std::cout << "(空)";
        }
        std::cout << std::endl;
    }

    std::string getName() const { return name; }
    bool isEmpty() const { return data == nullptr; }
};

// ============================================
// 1. 左值 vs 右值
// ============================================

void demo_lvalue_rvalue() {
    std::cout << "\n--- 1. 左值 vs 右值 ---\n" << std::endl;

    int a = 10;    // a 是左值，10 是右值
    int b = a + 5; // b 是左值，a + 5 是右值

    std::cout << "a = " << a << " (左值)" << std::endl;
    std::cout << "a + 5 = " << (a + 5) << " (右值表达式)" << std::endl;

    // 左值引用
    int& lref = a;
    lref = 20;
    std::cout << "通过左值引用修改后 a = " << a << std::endl;

    // 右值引用
    int&& rref = 100;  // 绑定到临时对象
    std::cout << "右值引用 rref = " << rref << std::endl;

    // 右值引用也可以绑定到表达式
    int&& rref2 = a + 5;
    std::cout << "右值引用绑定表达式: " << rref2 << std::endl;
}

// ============================================
// 2. 移动构造函数
// ============================================

void demo_move_constructor() {
    std::cout << "\n--- 2. 移动构造函数 ---\n" << std::endl;

    std::cout << "创建原始对象:" << std::endl;
    BigData original("Original", 100);
    original.display();

    std::cout << "\n拷贝构造 (深拷贝):" << std::endl;
    BigData copied = original;  // 调用拷贝构造
    copied.display();
    original.display();  // original 仍然有效

    std::cout << "\n移动构造 (转移资源):" << std::endl;
    BigData moved = std::move(original);  // 调用移动构造
    moved.display();
    original.display();  // original 已被掏空
}

// ============================================
// 3. std::move
// ============================================

void demo_std_move() {
    std::cout << "\n--- 3. std::move ---\n" << std::endl;

    std::string str1 = "Hello";
    std::string str2;

    std::cout << "str1: " << str1 << ", str2: " << str2 << std::endl;

    // 不使用 move (拷贝)
    str2 = str1;
    std::cout << "拷贝后 str1: " << str1 << ", str2: " << str2 << std::endl;

    std::string str3;
    // 使用 move (移动)
    str3 = std::move(str1);
    std::cout << "移动后 str1: \"" << str1 << "\" (已空), str3: " << str3 << std::endl;

    // 在容器中使用
    std::vector<BigData> vec;
    vec.reserve(5);

    std::cout << "\n向 vector 添加元素:" << std::endl;
    BigData temp1("Temp1", 50);
    vec.push_back(temp1);  // 拷贝

    std::cout << "\n使用移动语义:" << std::endl;
    BigData temp2("Temp2", 50);
    vec.push_back(std::move(temp2));  // 移动

    std::cout << "\nvector 大小: " << vec.size() << std::endl;
}

// ============================================
// 4. 完美转发和 std::forward
// ============================================

template <typename T>
void process(T&& arg) {
    std::cout << "process 接收参数类型: ";
    if (std::is_rvalue_reference<T&&>::value) {
        std::cout << "右值引用" << std::endl;
    } else {
        std::cout << "左值引用" << std::endl;
    }
}

// 正确的完美转发
template <typename T>
void perfect_forward(T&& arg) {
    std::cout << "perfect_forward: ";
    process(std::forward<T>(arg));
}

// ============================================
// 5. 返回值优化 (RVO/NRVO)
// ============================================

BigData create_big_data(const std::string& name, size_t size) {
    std::cout << "\n在 create_big_data 中:" << std::endl;
    BigData data(name, size);
    return data;  // RVO/NRVO 可能消除拷贝/移动
}

BigData create_big_data_return_move(const std::string& name, size_t size) {
    std::cout << "\n在 create_big_data_return_move 中:" << std::endl;
    BigData data(name, size);
    return std::move(data);  // 不推荐，会阻止 RVO
}

void demo_rvo() {
    std::cout << "\n--- 5. 返回值优化 (RVO) ---\n" << std::endl;

    std::cout << "普通返回 (可能触发 RVO):" << std::endl;
    auto data1 = create_big_data("RVO-Test", 200);

    std::cout << "\n返回 std::move (阻止 RVO):" << std::endl;
    auto data2 = create_big_data_return_move("No-RVO-Test", 200);
}

// ============================================
// 6. 容器中的移动语义
// ============================================

void demo_container_move() {
    std::cout << "\n--- 6. 容器中的移动语义 ---\n" << std::endl;

    std::vector<BigData> vec1, vec2;

    std::cout << "填充 vec1:" << std::endl;
    vec1.emplace_back("Data1", 100);
    vec1.emplace_back("Data2", 100);
    vec1.emplace_back("Data3", 100);

    std::cout << "\n赋值操作 vec2 = std::move(vec1):" << std::endl;
    vec2 = std::move(vec1);  // O(1) 移动

    std::cout << "vec2 大小: " << vec2.size() << std::endl;
    std::cout << "vec1 大小: " << vec1.size() << " (已移动)" << std::endl;

    std::cout << "\nstd::swap (使用移动语义):" << std::endl;
    std::vector<BigData> vec3;
    vec3.emplace_back("Swap-Data", 50);

    std::cout << "交换 vec2 和 vec3:" << std::endl;
    std::swap(vec2, vec3);

    std::cout << "vec2 大小: " << vec2.size() << std::endl;
    std::cout << "vec3 大小: " << vec3.size() << std::endl;
}

// ============================================
// 7. 移动语义的性能对比
// ============================================

#include <chrono>

void demo_performance() {
    std::cout << "\n--- 7. 性能对比 ---\n" << std::endl;

    const size_t count = 5000;
    const size_t data_size = 10000;  // 40 KB per object
    const bool silent_mode = true;

    std::cout << "测试配置: " << count << " 次循环, 数据大小: " << data_size << " 个 int (约 " << data_size * sizeof(int) / 1024 << " KB)" << std::endl;

    // 方案 1: 拷贝 - vector扩容时的拷贝开销
    std::cout << "\n测试1: vector 扩容时的拷贝 vs 移动" << std::endl;

    // 拷贝测试 - vector扩容时会拷贝所有元素
    auto start_copy = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec_copy;
        for (size_t i = 0; i < count; ++i) {
            vec_copy.push_back(BigData("Data", data_size, silent_mode));  // 可能触发拷贝（扩容时）
        }
    }
    auto end_copy = std::chrono::high_resolution_clock::now();
    auto copy_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_copy - start_copy);

    // 移动测试 - 使用 reserve 和 move 避免拷贝
    auto start_move = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec_move;
        vec_move.reserve(count);  // 预分配空间
        for (size_t i = 0; i < count; ++i) {
            vec_move.push_back(BigData("Data", data_size, silent_mode));  // 移动，无拷贝
        }
    }
    auto end_move = std::chrono::high_resolution_clock::now();
    auto move_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_move - start_move);

    std::cout << "\n结果对比:" << std::endl;
    std::cout << "  拷贝 (vector扩容): " << copy_duration.count() << " ms" << std::endl;
    std::cout << "  移动 (预分配+move): " << move_duration.count() << " ms" << std::endl;
    if (move_duration.count() > 0) {
        std::cout << "  性能提升: " << static_cast<double>(copy_duration.count()) / move_duration.count() << "x" << std::endl;
        std::cout << "  时间节省: " << (copy_duration.count() - move_duration.count()) << " ms" << std::endl;
    }
}

// ============================================
// 8. 移动语义最佳实践
// ============================================

void demo_best_practices() {
    std::cout << "\n--- 8. 最佳实践 ---\n" << std::endl;

    // 1. 移动容器
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = std::move(vec1);
    std::cout << "移动容器 vec2 大小: " << vec2.size() << std::endl;

    // 2. 移动字符串
    std::string long_text(1000, 'A');
    std::string moved_text = std::move(long_text);
    std::cout << "移动字符串大小: " << moved_text.size() << std::endl;

    // 3. 使用 emplace_back
    std::vector<std::pair<int, std::string>> pairs;

    // 不推荐：构造临时对象再移动
    pairs.push_back(std::pair<int, std::string>(1, "One"));

    // 推荐：直接在容器中构造
    pairs.emplace_back(1, "One");

    // 4. 传值 vs 传引用
    auto func_by_value = [](std::string s) {
        // 参数被拷贝或移动
        return s;
    };

    auto func_by_ref = [](const std::string& s) -> std::string {
        // 只读，返回时可能移动
        return s;
    };

    std::string text = "Hello, World!";
    std::string result1 = func_by_value(text);        // 拷贝
    std::string result2 = func_by_value(std::move(text));  // 移动
    std::string result3 = func_by_ref(text);          // 引用 + 移动返回值

    std::cout << "最佳实践示例完成" << std::endl;
}

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ 移动语义实验" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 左值 vs 右值
    demo_lvalue_rvalue();

    // 2. 移动构造函数
    demo_move_constructor();

    // 3. std::move
    demo_std_move();

    // 4. 完美转发
    std::cout << "\n--- 4. 完美转发 ---\n" << std::endl;
    int x = 42;
    perfect_forward(x);       // 传递左值
    perfect_forward(100);     // 传递右值
    perfect_forward(std::move(x));  // 传递右值引用

    // 5. 返回值优化
    demo_rvo();

    // 6. 容器中的移动语义
    demo_container_move();

    // 7. 性能对比
    demo_performance();

    // 8. 最佳实践
    demo_best_practices();

    std::cout << "\n========================================" << std::endl;
    std::cout << "实验完成!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
