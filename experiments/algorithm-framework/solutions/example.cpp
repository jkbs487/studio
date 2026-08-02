/**
 * C++ 算法框架使用示例。
 *
 * 编译：
 *   g++ -std=c++17 -O2 -o main main.cpp solutions/example.cpp -I.
 * 运行：
 *   ./main two_sum --bench
 *   ./main factorial
 *   ./main --bench
 */

#include <algorithm>
#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "../algoframe.hpp"

/* ============================================================
 * 算法 1：两数之和
 * ============================================================ */

using TwoSumResult = std::optional<std::pair<int, int>>;

TwoSumResult two_sum(const std::vector<int>& nums, int target) {
    std::map<int, int> seen;
    for (int i = 0; i < static_cast<int>(nums.size()); i++) {
        int complement = target - nums[i];
        if (seen.count(complement)) {
            return std::make_pair(seen[complement], i);
        }
        seen[nums[i]] = i;
    }
    return std::nullopt;
}

/* ============================================================
 * 算法 2：阶乘
 * ============================================================ */

long long factorial(int n) {
    if (n < 0) throw std::invalid_argument("n must be non-negative");
    long long result = 1;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}

/* ============================================================
 * 注册入口（由 main.cpp 调用）
 * ============================================================ */

void register_example_algorithms() {
    using namespace algoframe;

    // --- 两数之和 ---
    register_algo("two_sum", "两数之和：返回和为 target 的两个下标");

    CaseBuilder("two_sum")
        .add("basic",
             []() { return two_sum({2, 7, 11, 15}, 9); },
             std::make_pair(0, 1))
        .add("no-duplicate",
             []() { return two_sum({3, 2, 4}, 6); },
             std::make_pair(1, 2))
        .add("duplicate",
             []() { return two_sum({3, 3}, 6); },
             std::make_pair(0, 1))
        .add("no-solution",
             []() { return two_sum({1, 5, 3}, 10); },
             TwoSumResult(std::nullopt))
        .finish();

    CaseBuilder("two_sum", /*bench=*/true)
        .add("large-array",
             []() {
                 std::vector<int> nums(100000);
                 for (int i = 0; i < 100000; i++) nums[i] = i;
                 return two_sum(nums, 199997);
             },
             std::make_pair(99998, 99999))
        .finish();

    // --- 阶乘 ---
    register_algo("factorial", "阶乘：返回 n!");

    CaseBuilder("factorial")
        .add("zero", []() { return factorial(0); }, 1LL)
        .add("one", []() { return factorial(1); }, 1LL)
        .add("five", []() { return factorial(5); }, 120LL)
        .add("ten", []() { return factorial(10); }, 3628800LL)
        .finish();
}
