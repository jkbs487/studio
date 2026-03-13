#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>

// ============================================================================
// 辅助函数：打印视图（不复制）
// ============================================================================
void printVector(const std::string& name, const std::vector<int>& v) {
    std::cout << name << ": [";
    bool first = true;
    for (const auto& elem : v) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

void printVectorDouble(const std::string& name, const std::vector<double>& v) {
    std::cout << name << ": [";
    bool first = true;
    for (const auto& elem : v) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

// ============================================================================
// 1. 基础 range 和视图
// ============================================================================
void basicRanges() {
    std::cout << "\n========== 1. 基础 Range ==========\n";

    // 传统容器也是 range
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 使用 views 转换
    std::vector<int> doubled;
    for (int x : vec | std::views::transform([](int x) { return x * 2; })) {
        doubled.push_back(x);
    }
    printVector("原始向量", vec);
    printVector("2倍", doubled);

    // views::filter - 过滤元素
    std::vector<int> evens;
    for (int x : vec | std::views::filter([](int x) { return x % 2 == 0; })) {
        evens.push_back(x);
    }
    printVector("偶数", evens);

    // 管道操作符组合
    std::vector<int> result;
    for (int x : vec
        | std::views::filter([](int x) { return x > 2; })
        | std::views::transform([](int x) { return x * x; })) {
        result.push_back(x);
    }
    printVector("x > 2 -> x^2", result);
}

// ============================================================================
// 2. 视图工厂 (View Factories)
// ============================================================================
void viewFactories() {
    std::cout << "\n========== 2. 视图工厂 ==========\n";

    // views::iota - 生成整数序列
    std::vector<int> iotaVec;
    for (int x : std::views::iota(1, 10)) {  // [1, 2, ..., 9]
        iotaVec.push_back(x);
    }
    printVector("iota(1, 10)", iotaVec);

    // 无限序列 (使用 take 限制)
    std::vector<int> infinite;
    for (int x : std::views::iota(1) | std::views::take(10)) {
        infinite.push_back(x);
    }
    printVector("无限序列 take(10)", infinite);

    // views::single - 单元素视图
    auto singleView = std::views::single(42);
    std::vector<int> singleVec(singleView.begin(), singleView.end());
    printVector("views::single(42)", singleVec);
}

// ============================================================================
// 3. 视图适配器 (View Adapters)
// ============================================================================
void viewAdapters() {
    std::cout << "\n========== 3. 视图适配器 ==========\n";

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // views::take - 取前 n 个
    std::vector<int> take3;
    for (int x : vec | std::views::take(3)) take3.push_back(x);
    printVector("take(3)", take3);

    // views::drop - 跳过前 n 个
    std::vector<int> drop3;
    for (int x : vec | std::views::drop(3)) drop3.push_back(x);
    printVector("drop(3)", drop3);

    // views::reverse - 反转
    std::vector<int> reversed;
    for (int x : vec | std::views::reverse) reversed.push_back(x);
    printVector("reverse", reversed);

    // views::split - 分割
    std::string str = "hello,world";
    std::cout << "split(','): ";
    for (auto part : str | std::views::split(',')) {
        std::cout << std::string(part.begin(), part.end()) << " ";
    }
    std::cout << "\n";

    // views::join - 连接 (使用模拟方式)
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5}};
    std::vector<int> joined;
    for (const auto& inner : nested) {
        for (int x : inner) joined.push_back(x);
    }
    printVector("join (模拟)", joined);
}

// ============================================================================
// 4. Ranges 算法
// ============================================================================
void rangesAlgorithms() {
    std::cout << "\n========== 4. Ranges 算法 ==========\n";

    std::vector<int> vec = {5, 2, 8, 1, 9, 3};

    // 排序 - 使用 std::sort
    std::vector<int> sorted = vec;
    std::sort(sorted.begin(), sorted.end());
    printVector("sort", sorted);

    // 查找 - 使用 std::find
    auto it = std::find(vec.begin(), vec.end(), 8);
    if (it != vec.end()) {
        std::cout << "find(8): 找到 " << *it << "\n";
    }

    // 计数 - 使用 std::count_if
    vec = {1, 2, 2, 3, 2, 4};
    auto cnt = std::count_if(vec.begin(), vec.end(), [](int x) { return x == 2; });
    std::cout << "count(2): " << cnt << "\n";

    // 条件查找 - 使用 std::find_if
    auto it2 = std::find_if(vec.begin(), vec.end(), [](int x) { return x > 3; });
    if (it2 != vec.end()) {
        std::cout << "find_if(x > 3): " << *it2 << "\n";
    }

    // 范围检查 - 使用 std:: 版本
    std::cout << "all_of > 0: " << std::all_of(vec.begin(), vec.end(), [](int x) { return x > 0; }) << "\n";
    std::cout << "any_of > 5: " << std::any_of(vec.begin(), vec.end(), [](int x) { return x > 5; }) << "\n";

    // min/max
    std::cout << "min_element: " << *std::min_element(vec.begin(), vec.end()) << "\n";
    std::cout << "max_element: " << *std::max_element(vec.begin(), vec.end()) << "\n";

    // accumulate
    std::cout << "accumulate: " << std::accumulate(vec.begin(), vec.end(), 0) << "\n";
}

// ============================================================================
// 5. 投影 (Projections)
// ============================================================================
void projections() {
    std::cout << "\n========== 5. 投影 ==========\n";

    struct Person {
        std::string name;
        int age;
    };

    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    // 按 age 排序
    std::vector<Person> sortedPeople = people;
    std::sort(sortedPeople.begin(), sortedPeople.end(),
         [](const Person& a, const Person& b) { return a.age < b.age; });
    std::cout << "按 age 排序: ";
    for (const auto& p : sortedPeople) {
        std::cout << p.name << "(" << p.age << ") ";
    }
    std::cout << "\n";

    // 按 name 排序
    sortedPeople = people;
    std::sort(sortedPeople.begin(), sortedPeople.end(),
         [](const Person& a, const Person& b) { return a.name < b.name; });
    std::cout << "按 name 排序: ";
    for (const auto& p : sortedPeople) {
        std::cout << p.name << " ";
    }
    std::cout << "\n";

    // max with comparator
    Person oldest = *std::max_element(people.begin(), people.end(),
        [](const Person& a, const Person& b) { return a.age < b.age; });
    std::cout << "最年长: " << oldest.name << "\n";
}

// ============================================================================
// 6. 惰性求值示例
// ============================================================================
void lazyEvaluation() {
    std::cout << "\n========== 6. 惰性求值 ==========\n";

    std::vector<int> data = {1, 2, 3, 4, 5};

    // 创建视图链 - 不立即计算
    auto pipeline = data
        | std::views::filter([](int x) { return x % 2 == 1; })
        | std::views::transform([](int x) { return x * 2; })
        | std::views::take(2);

    std::cout << "创建视图链 (尚未计算)\n";

    // 迭代时才计算
    std::cout << "计算结果: ";
    for (int x : pipeline) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 对同一个视图多次迭代
    std::cout << "再次迭代: ";
    for (int x : pipeline) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

// ============================================================================
// 7. 实际应用场景
// ============================================================================
void realWorldScenarios() {
    std::cout << "\n========== 7. 实际应用场景 ==========\n";

    // 场景1: 数据处理管道
    std::cout << "场景1: 数据处理管道\n";
    std::vector<double> prices = {100.0, 250.0, 50.0, 300.0, 75.0};

    // 过滤 > 100, 打9折, 四舍五入, 排序
    std::vector<double> discounted;
    for (double p : prices
        | std::views::filter([](double x) { return x > 100; })
        | std::views::transform([](double x) { return std::round(x * 0.9); })) {
        discounted.push_back(p);
    }
    std::sort(discounted.begin(), discounted.end(), std::greater<double>());
    printVectorDouble("折扣后价格", discounted);

    // 场景2: 字符串处理
    std::cout << "\n场景2: 字符串处理\n";
    std::string text = "Hello World C++20 Ranges are awesome";

    // 找出所有单词，以大写形式输出
    std::vector<std::string> words;
    for (auto word : text | std::views::split(' ') | std::views::transform([](auto w) {
        std::string s(w.begin(), w.end());
        for (char& c : s) c = std::toupper(c);
        return s;
    })) {
        words.push_back(word);
    }

    std::cout << "单词 (大写): ";
    for (const auto& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n";

    // 场景3: 组合多个操作
    std::cout << "\n场景3: 组合操作\n";
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 找前3个偶数的平方和
    int sum = 0;
    int count = 0;
    for (int x : numbers | std::views::filter([](int x) { return x % 2 == 0; })) {
        if (count >= 3) break;
        sum += x * x;
        ++count;
    }
    std::cout << "前3个偶数的平方和: " << sum << "\n";
}

// ============================================================================
// 8. 概念和约束
// ============================================================================
void conceptsAndConstraints() {
    std::cout << "\n========== 8. 概念和约束 ==========\n";

    // 检查类型是否符合 range 概念
    std::vector<int> vec = {1, 2, 3};

    // range 概念
    static_assert(std::ranges::range<decltype(vec)>, "vector 是 range");
    static_assert(std::ranges::sized_range<decltype(vec)>, "vector 是 sized_range");

    // 检查视图
    auto view = vec | std::views::transform([](int x) { return x * 2; });
    static_assert(std::ranges::view<decltype(view)>, "transform view 是 view");
    static_assert(std::ranges::range<decltype(view)>, "transform view 是 range");

    // forward_range
    static_assert(std::ranges::forward_range<decltype(vec)>, "vector 是 forward_range");

    std::cout << "概念检查通过\n";
}

// ============================================================================
// 9. 自定义 Range (使用 iota)
// ============================================================================
void customRange() {
    std::cout << "\n========== 9. 自定义 Range ==========\n";

    // 使用 iota 替代 generate
    std::vector<int> custom;
    for (int n : std::views::iota(1, 6)
        | std::views::transform([](int n) { return n * n * n; })) {
        custom.push_back(n);
    }
    printVector("1^3 到 5^3", custom);

    // 模拟 repeat + take
    std::vector<int> repeated;
    for (int i = 0; i < 5; ++i) {
        repeated.push_back(7);
    }
    printVector("repeat(7) take(5)", repeated);
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "       C++20 Ranges 演示\n";
    std::cout << "========================================\n";

    basicRanges();
    viewFactories();
    viewAdapters();
    rangesAlgorithms();
    projections();
    lazyEvaluation();
    realWorldScenarios();
    conceptsAndConstraints();
    customRange();

    std::cout << "\n========================================\n";
    std::cout << "          演示完成\n";
    std::cout << "========================================\n";

    return 0;
}
