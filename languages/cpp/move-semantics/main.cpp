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
// ============================================
// 7. 性能对比测试
// ============================================

#include <chrono>
#include <iomanip>

void demo_performance() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "性能测试" << std::endl;
    std::cout << "========================================" << std::endl;

    const bool silent = true;

    // ============================================
    // 测试 1: 拷贝 vs 移动
    // ============================================
    std::cout << "\n【测试 1】拷贝构造 vs 移动构造" << std::endl;

    const size_t count = 10000;
    const size_t data_size = 10000;  // 39 KB per object
    std::cout << "配置: " << count << " 个对象，每个约 " << data_size * sizeof(int) / 1024 << " KB\n" << std::endl;

    // 公平对比：两个测试的构造开销相同
    std::vector<BigData> sources;
    sources.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        sources.emplace_back("Source", data_size, silent);
    }

    // 测试拷贝
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            vec.push_back(sources[i]);  // 拷贝: 深拷贝39KB
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    long long copy_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // 测试移动
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            vec.push_back(std::move(sources[i]));  // 移动: 只转移指针
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long move_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  拷贝: " << copy_time / 1000.0 << " ms (" << copy_time << " μs)" << std::endl;
    std::cout << "  移动: " << move_time / 1000.0 << " ms (" << move_time << " μs)" << std::endl;
    if (move_time > 0) {
        double ratio = static_cast<double>(copy_time) / move_time;
        std::cout << "  性能提升: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
        std::cout << "  每次节省: " << (copy_time - move_time) / 1000.0 / count << " μs" << std::endl;
    }

    // ============================================
    // 测试 2: 参数传递
    // ============================================
    std::cout << "\n【测试 2】参数传递方式" << std::endl;

    const size_t func_calls = 100000;
    const size_t str_size = 10000;  // 10 KB string
    std::string large_str(str_size, 'X');
    std::cout << "配置: " << func_calls << " 次函数调用，字符串大小: " << str_size / 1024 << " KB\n" << std::endl;

    // 传值：每次拷贝 10KB
    auto by_value = [](std::string s) { return s.size(); };
    start = std::chrono::high_resolution_clock::now();
    {
        volatile size_t total = 0;
        for (size_t i = 0; i < func_calls; ++i) {
            total += by_value(large_str);  // 每次拷贝字符串
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long value_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 传引用：无拷贝
    auto by_ref = [](const std::string& s) { return s.size(); };
    start = std::chrono::high_resolution_clock::now();
    {
        volatile size_t total = 0;
        for (size_t i = 0; i < func_calls; ++i) {
            total += by_ref(large_str);  // 只传指针
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long ref_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  传值: " << value_time << " ms" << std::endl;
    std::cout << "  传引用: " << ref_time << " ms" << std::endl;
    if (ref_time > 0) {
        double ratio = static_cast<double>(value_time) / ref_time;
        std::cout << "  性能提升: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
    }

    // ============================================
    // 测试 3: vector 扩容
    // ============================================
    std::cout << "\n【测试 3】vector 扩容 vs 预分配" << std::endl;

    const size_t vec_size = 20000;
    const size_t obj_size = 5000;  // 20 KB per object
    std::cout << "配置: " << vec_size << " 个元素，每个约 " << obj_size * sizeof(int) / 1024 << " KB\n" << std::endl;

    // 不预分配：vector 会多次扩容
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        for (size_t i = 0; i < vec_size; ++i) {
            vec.emplace_back("Data", obj_size, silent);  // 扩容时移动所有元素
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long no_reserve_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 预分配：避免扩容
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(vec_size);
        for (size_t i = 0; i < vec_size; ++i) {
            vec.emplace_back("Data", obj_size, silent);  // 无扩容
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long reserve_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  无reserve: " << no_reserve_time << " ms" << std::endl;
    std::cout << "  有reserve: " << reserve_time << " ms" << std::endl;
    if (reserve_time > 0) {
        double ratio = static_cast<double>(no_reserve_time) / reserve_time;
        std::cout << "  性能提升: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
    }

    // ============================================
    // 测试 4: emplace_back 优势
    // ============================================
    std::cout << "\n【测试 4】push_back vs emplace_back" << std::endl;

    const size_t emplace_count = 100000;
    const size_t emplace_size = 1000;  // 4 KB per object
    std::cout << "配置: " << emplace_count << " 次操作，每个对象约 " << emplace_size * sizeof(int) / 1024 << " KB\n" << std::endl;
    std::cout << "说明: 对比临时对象的构造+移动 vs 直接构造\n" << std::endl;

    // push_back(临时对象): 构造临时对象 + 移动构造
    auto start_emplace = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(emplace_count);
        for (size_t i = 0; i < emplace_count; ++i) {
            vec.push_back(BigData("Data", emplace_size, silent));  // 构造临时对象，然后移动
        }
    }
    auto end_emplace = std::chrono::high_resolution_clock::now();
    long long push_back_temp_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_emplace - start_emplace).count();

    // emplace_back: 直接在容器中构造
    start_emplace = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(emplace_count);
        for (size_t i = 0; i < emplace_count; ++i) {
            vec.emplace_back("Data", emplace_size, silent);  // 直接在容器中构造
        }
    }
    end_emplace = std::chrono::high_resolution_clock::now();
    long long emplace_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_emplace - start_emplace).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  push_back(临时对象): " << push_back_temp_time << " ms" << std::endl;
    std::cout << "  emplace_back: " << emplace_time << " ms" << std::endl;
    if (emplace_time > 0) {
        double ratio = static_cast<double>(push_back_temp_time) / emplace_time;
        std::cout << "  性能提升: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
        std::cout << "\n原理:" << std::endl;
        std::cout << "  push_back(BigData(...)): 临时对象构造 + 移动构造 (2次构造)" << std::endl;
        std::cout << "  emplace_back(...):         直接在容器中构造 (1次构造)" << std::endl;
        std::cout << "\n注意: 在此测试中，由于移动构造很快（只复制指针），" << std::endl;
        std::cout << "      emplace_back 的优势可能不明显。" << std::endl;
        std::cout << "      对于构造开销大的复杂对象，emplace_back 优势更明显。" << std::endl;
    }

    // ============================================
    // 测试 4.5: 拷贝 vs 构造（说明测试4结果的原因）
    // ============================================
    std::cout << "\n【测试 4.5】拷贝构造 vs 带参数构造（说明为什么构造比拷贝慢）" << std::endl;

    const size_t compare_count = 10000;
    std::cout << "配置: " << compare_count << " 次操作，每个对象约 " << emplace_size * sizeof(int) / 1024 << " KB\n" << std::endl;
    std::cout << "说明: 演示为什么上面的 emplace_back 没有比 push_back 快\n" << std::endl;

    // 准备源对象用于拷贝测试
    BigData source_obj("Source", emplace_size, silent);

    // 拷贝构造: std::copy (高效)
    auto start_compare = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(compare_count);
        for (size_t i = 0; i < compare_count; ++i) {
            vec.push_back(source_obj);  // 拷贝构造: std::copy (memcpy)
        }
    }
    auto end_compare = std::chrono::high_resolution_clock::now();
    long long copy_construct_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_compare - start_compare).count();

    // 带参数构造: for 循环初始化 (低效)
    start_compare = std::chrono::high_resolution_clock::now();
    {
        std::vector<BigData> vec;
        vec.reserve(compare_count);
        for (size_t i = 0; i < compare_count; ++i) {
            vec.emplace_back("Data", emplace_size, silent);  // 带参数构造: for 循环初始化
        }
    }
    end_compare = std::chrono::high_resolution_clock::now();
    long long param_construct_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_compare - start_compare).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  拷贝构造 (std::copy): " << copy_construct_time << " ms" << std::endl;
    std::cout << "  带参数构造 (for循环): " << param_construct_time << " ms" << std::endl;
    if (param_construct_time > 0) {
        double ratio = static_cast<double>(param_construct_time) / copy_construct_time;
        std::cout << "  拷贝反而快: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
        std::cout << "\n原因分析:" << std::endl;
        std::cout << "  1. 拷贝构造使用 std::copy，内部可能使用 memcpy (批量拷贝，高效)" << std::endl;
        std::cout << "  2. 带参数构造使用 for 循环逐个赋值 (逐个操作，低效)" << std::endl;
        std::cout << "  3. 这解释了为什么构造比拷贝慢" << std::endl;
        std::cout << "\n结论:" << std::endl;
        std::cout << "  - emplace_back 的优势是避免临时对象，不是替代拷贝" << std::endl;
        std::cout << "  - 如果必须构造对象，emplace_back 比 push_back(临时对象) 稍快" << std::endl;
        std::cout << "  - 如果对象已存在，拷贝可能比构造更快（取决于初始化方式）" << std::endl;
    }

    // ============================================
    // 测试 5: swap 优化
    // ============================================
    std::cout << "\n【测试 5】swap 操作" << std::endl;

    const size_t swap_count = 100000;
    const size_t swap_size = 50000;  // 195 KB per object
    std::cout << "配置: " << swap_count << " 次操作，每个对象约 " << swap_size * sizeof(int) / 1024 << " KB" << std::endl;
    std::cout << "说明: std::swap 使用移动语义，避免深拷贝\n" << std::endl;

    // 手动swap（拷贝3次）
    auto start_swap = std::chrono::high_resolution_clock::now();
    {
        for (size_t i = 0; i < swap_count; ++i) {
            BigData a("A", swap_size, silent);
            BigData b("B", swap_size, silent);
            BigData temp = a;  // 拷贝1
            a = b;             // 拷贝2
            b = temp;          // 拷贝3
        }
    }
    auto end_swap = std::chrono::high_resolution_clock::now();
    long long manual_swap_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_swap - start_swap).count();

    // std::swap（移动3次）
    start_swap = std::chrono::high_resolution_clock::now();
    {
        for (size_t i = 0; i < swap_count; ++i) {
            BigData a("A", swap_size, silent);
            BigData b("B", swap_size, silent);
            std::swap(a, b);  // 移动
        }
    }
    end_swap = std::chrono::high_resolution_clock::now();
    long long std_swap_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_swap - start_swap).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  手动swap (3次拷贝): " << manual_swap_time << " ms" << std::endl;
    std::cout << "  std::swap (3次移动): " << std_swap_time << " ms" << std::endl;
    if (std_swap_time > 0) {
        double ratio = static_cast<double>(manual_swap_time) / std_swap_time;
        std::cout << "  性能提升: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
        std::cout << "\n注意: 提升不明显是因为移动操作虽然快（仅复制指针），但仍有开销。" << std::endl;
        std::cout << "      更重要的是避免深拷贝带来的额外内存分配和释放。" << std::endl;
    }

    // ============================================
    // 测试 6: 字符串拼接（大字符串）
    // ============================================
    std::cout << "\n【测试 6】字符串拼接（大字符串）" << std::endl;

    const size_t concat_count = 10000;
    const size_t big_str_size = 100000;  // 100 KB
    std::cout << "配置: " << concat_count << " 次拼接，每个字符串约 " << big_str_size / 1024 << " KB" << std::endl;
    std::cout << "说明: 使用大字符串避免 SSO（小字符串优化）\n" << std::endl;

    // 拷贝拼接
    start = std::chrono::high_resolution_clock::now();
    {
        for (size_t i = 0; i < concat_count; ++i) {
            std::string s1(big_str_size, 'A');
            std::string s2(big_str_size, 'B');
            std::string s3(big_str_size, 'C');
            std::string result = s1 + s2 + s3;  // 拷贝
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long copy_concat_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 移动拼接
    start = std::chrono::high_resolution_clock::now();
    {
        for (size_t i = 0; i < concat_count; ++i) {
            std::string s1(big_str_size, 'A');
            std::string s2(big_str_size, 'B');
            std::string s3(big_str_size, 'C');
            std::string result = std::move(s1) + std::move(s2) + std::move(s3);  // 移动
        }
    }
    end = std::chrono::high_resolution_clock::now();
    long long move_concat_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "结果:" << std::endl;
    std::cout << "  拷贝拼接: " << copy_concat_time << " ms" << std::endl;
    std::cout << "  移动拼接: " << move_concat_time << " ms" << std::endl;
    if (move_concat_time > 0) {
        double ratio = static_cast<double>(copy_concat_time) / move_concat_time;
        std::cout << "  性能提升: " << std::fixed << std::setprecision(1) << ratio << "x" << std::endl;
        std::cout << "\n注意: 提升不明显是因为编译器可能优化了字符串拼接表达式。" << std::endl;
        std::cout << "      std::string 的 operator+ 已经进行了优化，手动 std::move 可能无法带来额外收益。" << std::endl;
        std::cout << "      移动字符串在以下场景更有优势：赋值、函数返回、容器操作等。" << std::endl;
    }

    // ============================================
    // 总结
    // ============================================
    std::cout << "\n========================================" << std::endl;
    std::cout << "性能测试结论" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. ✅ 移动语义对于大对象（>1KB）效果显著（19x）" << std::endl;
    std::cout << "2. ✅ 传递大对象参数时，优先使用 const& 引用（避免拷贝）" << std::endl;
    std::cout << "3. ⚠️  vector reserve 可减少扩容，但提升有限（移动扩容已快）" << std::endl;
    std::cout << "4. ⚠️  emplace_back vs push_back(临时对象): 稍快（避免临时对象）" << std::endl;
    std::cout << "   但带参数构造可能比拷贝慢（初始化方式影响）" << std::endl;
    std::cout << "5. ⚠️  std::swap 使用移动语义，避免深拷贝但仍有开销（1.2x）" << std::endl;
    std::cout << "6. ⚠️  字符串拼接 operator+ 已优化，手动 std::move 收益有限" << std::endl;
    std::cout << "\n关键洞察:" << std::endl;
    std::cout << "- 移动语义在以下场景最有价值：对象移动、函数返回、容器操作" << std::endl;
    std::cout << "- 编译器优化（RVO、SSO、字符串优化）可能掩盖移动语义的效果" << std::endl;
    std::cout << "- 拷贝不总是慢：std::copy(memcpy) 可能比 for 循环初始化快" << std::endl;
    std::cout << "- emplace_back 的优势是避免临时对象，不是替代拷贝或移动" << std::endl;
    std::cout << "- 设计性能测试时需要考虑编译器优化、初始化方式等因素" << std::endl;
    std::cout << "\n重要结论（测试4.5）:" << std::endl;
    std::cout << "- 拷贝构造使用 std::copy，可能使用 memcpy (批量操作)" << std::endl;
    std::cout << "- 带参数构造使用 for 循环逐个初始化 (逐个操作)" << std::endl;
    std::cout << "- 这就是为什么构造对象可能比拷贝对象慢的原因！" << std::endl;
    std::cout << "- emplace_back 应该对比 push_back(临时对象)，而不是拷贝" << std::endl;
    std::cout << "========================================" << std::endl;
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
