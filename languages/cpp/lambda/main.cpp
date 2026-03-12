#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>
#include <map>

// ============================================
// 1. Lambda 基础语法
// ============================================

void demo_basic_lambda() {
    std::cout << "\n--- 1. Lambda 基础语法 ---\n" << std::endl;

    // 无参数，无返回值
    auto greet = []() {
        std::cout << "Hello from Lambda!" << std::endl;
    };
    greet();

    // 有参数
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    std::cout << "add(10, 20) = " << add(10, 20) << std::endl;

    // 返回类型自动推导
    auto multiply = [](auto a, auto b) {
        return a * b;
    };
    std::cout << "multiply(3, 4) = " << multiply(3, 4) << std::endl;
    std::cout << "multiply(2.5, 4) = " << multiply(2.5, 4.0) << std::endl;

    // 捕获外部变量
    int x = 10;
    auto print_x = [x]() {  // 按值捕获
        std::cout << "捕获的 x = " << x << std::endl;
    };
    print_x();

    int y = 20;
    auto print_y_ref = [&y]() {  // 按引用捕获
        std::cout << "捕获的 y = " << y << std::endl;
    };
    y = 30;
    print_y_ref();

    // 混合捕获
    auto print_xy = [x, &y]() {
        std::cout << "x (值) = " << x << ", y (引用) = " << y << std::endl;
    };
    y = 40;
    print_xy();

    // 捕获所有
    int a = 5, b = 6;
    auto capture_all_value = [=]() {
        std::cout << "a = " << a << ", b = " << b << std::endl;
    };
    capture_all_value();

    auto capture_all_ref = [&]() {
        std::cout << "a = " << a << ", b = " << b << std::endl;
        a = 10;
        b = 20;
    };
    capture_all_ref();
    std::cout << "修改后 a = " << a << ", b = " << b << std::endl;

    // 可变 Lambda（修改按值捕获的变量）
    int counter = 0;
    auto mutable_lambda = [counter]() mutable {
        counter++;
        std::cout << "计数器: " << counter << std::endl;
    };
    mutable_lambda();
    mutable_lambda();
    std::cout << "外部 counter 仍然是: " << counter << std::endl;
}

// ============================================
// 2. Lambda 与 STL 算法
// ============================================

void demo_lambda_with_stl() {
    std::cout << "\n--- 2. Lambda 与 STL 算法 ---\n" << std::endl;

    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    std::cout << "原始数组: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // for_each - 打印所有元素
    std::cout << "for_each 打印: ";
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << n << " ";
    });
    std::cout << std::endl;

    // 排序 - 自定义比较
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end(), [](int a, int b) {
        return a > b;  // 降序
    });
    std::cout << "降序排序: ";
    for (int n : sorted) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // find_if - 查找第一个大于 5 的数
    auto it = std::find_if(numbers.begin(), numbers.end(), [](int n) {
        return n > 5;
    });
    if (it != numbers.end()) {
        std::cout << "第一个大于 5 的数: " << *it << std::endl;
    }

    // count_if - 计算偶数个数
    int even_count = std::count_if(numbers.begin(), numbers.end(), [](int n) {
        return n % 2 == 0;
    });
    std::cout << "偶数个数: " << even_count << std::endl;

    // transform - 转换元素
    std::vector<int> squares;
    std::transform(numbers.begin(), numbers.end(),
                   std::back_inserter(squares),
                   [](int n) { return n * n; });
    std::cout << "平方值: ";
    for (int n : squares) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // remove_if - 删除满足条件的元素
    numbers.erase(
        std::remove_if(numbers.begin(), numbers.end(), [](int n) {
            return n < 5;
        }),
        numbers.end()
    );
    std::cout << "删除小于5的数后: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // accumulate - 累积
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "总和: " << sum << std::endl;

    // 使用 Lambda 自定义累积
    int product = std::accumulate(numbers.begin(), numbers.end(),
                                   1, [](int acc, int n) {
        return acc * n;
    });
    std::cout << "乘积: " << product << std::endl;
}

// ============================================
// 3. 立即调用的 Lambda (IIFE)
// ============================================

void demo_iife() {
    std::cout << "\n--- 3. 立即调用的 Lambda ---\n" << std::endl;

    // 使用 IIFE 初始化 const 变量
    const auto result = []() {
        int x = 10;
        int y = 20;
        return x + y;
    }();
    std::cout << "IIFE 结果: " << result << std::endl;

    // 复杂的初始化逻辑
    const auto complex_init = []() {
        std::vector<int> vec;
        for (int i = 0; i < 10; ++i) {
            vec.push_back(i * i);
        }
        return vec;
    }();
    std::cout << "复杂初始化结果: ";
    for (int n : complex_init) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

// ============================================
// 4. 高阶函数
// ============================================

// 接受 Lambda 作为参数
void apply_operation(const std::vector<int>& vec,
                     const std::function<void(int)>& operation) {
    for (int n : vec) {
        operation(n);
    }
}

// 返回 Lambda
auto make_multiplier(int factor) {
    return [factor](int x) {
        return x * factor;
    };
}

// 柯里化
auto curry = [](auto func) {
    return [func](auto... args1) {
        return [func, args1...](auto... args2) {
            return func(args1..., args2...);
        };
    };
};

void demo_higher_order() {
    std::cout << "\n--- 4. 高阶函数 ---\n" << std::endl;

    // 接受 Lambda
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "应用操作: ";
    apply_operation(numbers, [](int n) {
        std::cout << n * 2 << " ";
    });
    std::cout << std::endl;

    // 返回 Lambda
    auto times2 = make_multiplier(2);
    auto times10 = make_multiplier(10);
    std::cout << "times2(5) = " << times2(5) << std::endl;
    std::cout << "times10(5) = " << times10(5) << std::endl;

    // 柯里化示例
    auto add = [](int a, int b) { return a + b; };
    auto curried_add = curry(add);
    auto add5 = curried_add(5);
    std::cout << "add5(10) = " << add5(10) << std::endl;
}

// ============================================
// 5. 事件处理模式
// ============================================

class Button {
private:
    std::function<void()> onClick;

public:
    void setCallback(std::function<void()> callback) {
        onClick = callback;
    }

    void click() {
        if (onClick) {
            onClick();
        }
    }
};

void demo_event_handling() {
    std::cout << "\n--- 5. 事件处理模式 ---\n" << std::endl;

    Button button;

    // 设置回调
    int clickCount = 0;
    button.setCallback([&clickCount]() {
        clickCount++;
        std::cout << "按钮被点击 " << clickCount << " 次" << std::endl;
    });

    // 模拟点击
    button.click();
    button.click();
    button.click();
}

// ============================================
// 6. Lambda 与多线程
// ============================================

#include <thread>
#include <mutex>

void demo_multithreading() {
    std::cout << "\n--- 6. Lambda 与多线程 ---\n" << std::endl;

    std::mutex mtx;
    int shared_counter = 0;

    auto worker = [&mtx, &shared_counter](int id, int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            shared_counter++;
            std::cout << "线程 " << id << ": 计数 = " << shared_counter << std::endl;
        }
    };

    std::thread t1(worker, 1, 5);
    std::thread t2(worker, 2, 5);

    t1.join();
    t2.join();

    std::cout << "最终计数: " << shared_counter << std::endl;
}

// ============================================
// 7. 函数式编程模式
// ============================================

template <typename T>
auto filter(const std::vector<T>& vec, auto predicate) {
    std::vector<T> result;
    for (const auto& item : vec) {
        if (predicate(item)) {
            result.push_back(item);
        }
    }
    return result;
}

template <typename T, typename U>
auto map(const std::vector<T>& vec, auto transform) {
    std::vector<U> result;
    for (const auto& item : vec) {
        result.push_back(transform(item));
    }
    return result;
}

template <typename T>
auto reduce(const std::vector<T>& vec, T initial, auto combine) {
    T result = initial;
    for (const auto& item : vec) {
        result = combine(result, item);
    }
    return result;
}

void demo_functional_programming() {
    std::cout << "\n--- 7. 函数式编程模式 ---\n" << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // filter - 过滤偶数
    auto evens = filter(numbers, [](int n) { return n % 2 == 0; });
    std::cout << "偶数: ";
    for (int n : evens) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // map - 平方
    auto squared = map<int, int>(numbers, [](int n) { return n * n; });
    std::cout << "平方: ";
    for (int n : squared) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // reduce - 求和
    int sum = reduce(numbers, 0, [](int acc, int n) { return acc + n; });
    std::cout << "总和: " << sum << std::endl;

    // 链式操作
    auto result = reduce(
        map<int, int>(
            filter(numbers, [](int n) { return n > 5; }),
            [](int n) { return n * 2; }
        ),
        0,
        [](int acc, int n) { return acc + n; }
    );
    std::cout << "大于5的数乘以2后求和: " << result << std::endl;
}

// ============================================
// 8. Lambda 递归
// ============================================

void demo_lambda_recursion() {
    std::cout << "\n--- 8. Lambda 递归 ---\n" << std::endl;

    // 使用 std::function 实现递归
    std::function<int(int)> factorial = [&factorial](int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    };

    std::cout << "factorial(5) = " << factorial(5) << std::endl;
    std::cout << "factorial(10) = " << factorial(10) << std::endl;
}

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ Lambda 表达式实验" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 基础 Lambda
    demo_basic_lambda();

    // 2. Lambda 与 STL
    demo_lambda_with_stl();

    // 3. IIFE
    demo_iife();

    // 4. 高阶函数
    demo_higher_order();

    // 5. 事件处理
    demo_event_handling();

    // 6. 多线程
    demo_multithreading();

    // 7. 函数式编程
    demo_functional_programming();

    // 8. Lambda 递归
    demo_lambda_recursion();

    std::cout << "\n========================================" << std::endl;
    std::cout << "实验完成!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
