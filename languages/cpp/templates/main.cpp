#include <iostream>
#include <string>
#include <type_traits>
#include <tuple>
#include <vector>
#include <array>

// ============================================
// 1. 函数模板
// ============================================

template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

// 多参数模板
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// ============================================
// 2. 类模板
// ============================================

template <typename T>
class Stack {
private:
    std::vector<T> elements;

public:
    void push(const T& value) {
        elements.push_back(value);
    }

    T pop() {
        if (elements.empty()) {
            throw std::out_of_range("Stack<>::pop(): empty stack");
        }
        T value = elements.back();
        elements.pop_back();
        return value;
    }

    bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }
};

// ============================================
// 3. 模板特化
// ============================================

// 通用模板
template <typename T>
class TypeInfo {
public:
    static void print() {
        std::cout << "未知类型" << std::endl;
    }
};

// 完全特化 - int
template <>
class TypeInfo<int> {
public:
    static void print() {
        std::cout << "类型: int (整数类型)" << std::endl;
    }
};

// 完全特化 - double
template <>
class TypeInfo<double> {
public:
    static void print() {
        std::cout << "类型: double (浮点类型)" << std::endl;
    }
};

// 偏特化 - 指针类型
template <typename T>
class TypeInfo<T*> {
public:
    static void print() {
        std::cout << "类型: " << typeid(T).name() << "* (指针类型)" << std::endl;
    }
};

// ============================================
// 4. 非类型模板参数
// ============================================

template <typename T, size_t Size>
class FixedArray {
private:
    std::array<T, Size> data;

public:
    FixedArray() : data{} {}

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const {
        return Size;
    }
};

// ============================================
// 5. 可变参数模板
// ============================================

// 编译时计算乘积
template <typename... Args>
auto multiply_all(Args... args) {
    return (args * ...);  // C++17 折叠表达式
}

// 打印可变参数
template <typename T>
void print_single(const T& value) {
    std::cout << value << " ";
}

template <typename First, typename... Rest>
void print_all(const First& first, const Rest&... rest) {
    std::cout << first << " ";
    if constexpr (sizeof...(rest) > 0) {
        print_all(rest...);
    }
}

// ============================================
// 6. SFINAE - 替换失败不是错误
// ============================================

// 只有当 T 是整数类型时才启用
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
only_for_integers(T value) {
    std::cout << "[整数] " << value << std::endl;
    return value * 2;
}

// 只有当 T 是浮点类型时才启用
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
only_for_integers(T value) {
    std::cout << "[浮点数] " << value << std::endl;
    return value / 2.0;
}

// C++20 使用 concepts 更简洁
template <typename T>
requires std::is_integral_v<T>
T square_integer(T value) {
    std::cout << "[概念-整数] 平方: ";
    return value * value;
}

// ============================================
// 7. 编译时计算（模板元编程）
// ============================================

// 编译时计算阶乘
template <unsigned int N>
struct Factorial {
    static constexpr unsigned int value = N * Factorial<N - 1>::value;
};

// 模板特化作为终止条件
template <>
struct Factorial<0> {
    static constexpr unsigned int value = 1;
};

// 编译时计算斐波那契数列
template <unsigned int N>
struct Fibonacci {
    static constexpr unsigned long long value =
        Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr unsigned long long value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr unsigned long long value = 1;
};

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ 模板实验" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 函数模板
    std::cout << "\n1. 函数模板:" << std::endl;
    std::cout << "max(10, 20) = " << max_value(10, 20) << std::endl;
    std::cout << "max(3.14, 2.71) = " << max_value(3.14, 2.71) << std::endl;
    std::cout << "add(10, 3.14) = " << add(10, 3.14) << std::endl;

    // 2. 类模板
    std::cout << "\n2. 类模板 (Stack<T>):" << std::endl;
    Stack<int> int_stack;
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    std::cout << "栈大小: " << int_stack.size() << std::endl;
    std::cout << "弹出的元素: " << int_stack.pop() << std::endl;
    std::cout << "栈大小: " << int_stack.size() << std::endl;

    Stack<std::string> string_stack;
    string_stack.push("Hello");
    string_stack.push("World");
    std::cout << "弹出的字符串: " << string_stack.pop() << std::endl;

    // 3. 模板特化
    std::cout << "\n3. 模板特化:" << std::endl;
    TypeInfo<int>::print();
    TypeInfo<double>::print();
    TypeInfo<std::string>::print();
    TypeInfo<int*>::print();

    // 4. 非类型模板参数
    std::cout << "\n4. 非类型模板参数:" << std::endl;
    FixedArray<int, 5> arr;
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = static_cast<int>(i * 10);
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }

    // 5. 可变参数模板
    std::cout << "\n5. 可变参数模板:" << std::endl;
    std::cout << "multiply_all(1, 2, 3, 4) = " << multiply_all(1, 2, 3, 4) << std::endl;
    std::cout << "multiply_all(2.5, 4) = " << multiply_all(2.5, 4.0) << std::endl;
    std::cout << "print_all: ";
    print_all("C++", "模板", "元编程", "很", "强大");
    std::cout << std::endl;

    // 6. SFINAE
    std::cout << "\n6. SFINAE 和类型约束:" << std::endl;
    only_for_integers(42);
    only_for_integers(3.14);
    std::cout << "square_integer(7) = " << square_integer(7) << std::endl;

    // 7. 模板元编程
    std::cout << "\n7. 编译时计算 (模板元编程):" << std::endl;
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Factorial<10>::value = " << Factorial<10>::value << std::endl;
    std::cout << "Fibonacci<10>::value = " << Fibonacci<10>::value << std::endl;
    std::cout << "Fibonacci<20>::value = " << Fibonacci<20>::value << std::endl;

    std::cout << "\n========================================" << std::endl;
    std::cout << "实验完成!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
