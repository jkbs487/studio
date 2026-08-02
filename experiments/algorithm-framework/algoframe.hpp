/**
 * 算法验证框架 — C++ 版本。
 *
 * 提供与 Python 版 algoframe.py 等价的注册 + 测试功能。
 *
 * 用法：
 *   1. 在 solutions/ 下创建 .cpp 文件
 *   2. 实现算法函数
 *   3. 在 main.cpp 的 register_all() 中注册算法和用例
 *   4. 编译并运行
 *
 * 示例见 solutions/example.cpp 和 main.cpp。
 */

#ifndef ALGOFRAME_HPP
#define ALGOFRAME_HPP

#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace algoframe {

/* ---------- Value formatting ---------- */

// Forward declarations
template <typename T>
std::string fmt_value(const T& v);
template <typename T>
std::string fmt_value(const std::optional<T>& v);
template <typename T1, typename T2>
std::string fmt_value(const std::pair<T1, T2>& v);

// Specializations (must come before the generic fallback)
inline std::string fmt_value(const std::string& v) { return "\"" + v + "\""; }
inline std::string fmt_value(bool v) { return v ? "true" : "false"; }

inline std::string fmt_value(double v) {
    std::ostringstream oss;
    oss << std::setprecision(10) << v;
    return oss.str();
}
inline std::string fmt_value(float v) {
    std::ostringstream oss;
    oss << std::setprecision(10) << v;
    return oss.str();
}

template <typename T>
std::string fmt_value(const std::optional<T>& v) {
    if (!v) return "None";
    return fmt_value(*v);
}

template <typename T1, typename T2>
std::string fmt_value(const std::pair<T1, T2>& v) {
    return "(" + fmt_value(v.first) + ", " + fmt_value(v.second) + ")";
}

// Fallback for arithmetic types
template <typename T>
std::string fmt_value(const T& v) {
    if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(v);
    } else {
        std::ostringstream oss;
        oss << v;
        return oss.str();
    }
}

/* ---------- Case ---------- */

struct Case {
    std::string name;
    std::function<std::string()> run;    // 执行算法并返回结果的字符串表示
    std::string expected_str;
    std::function<bool(const std::string&)> check;
};

/* ---------- Solution ---------- */

struct Solution {
    std::string name;
    std::string desc;
    std::vector<Case> cases;
    std::vector<Case> bench_cases;
};

/* ---------- Registry ---------- */

inline std::map<std::string, Solution>& registry() {
    static std::map<std::string, Solution> reg;
    return reg;
}

/* ---------- Registration helpers ---------- */

inline Solution& register_algo(const std::string& name, const std::string& desc) {
    auto& reg = registry();
    reg[name] = Solution{name, desc, {}, {}};
    return reg[name];
}

inline Solution& get_algo(const std::string& name) {
    return registry().at(name);
}

/* ---------- Case builder ---------- */

class CaseBuilder {
public:
    explicit CaseBuilder(std::string algo_name, bool is_bench = false)
        : algo_name_(std::move(algo_name)), is_bench_(is_bench) {}

    template <typename Fn, typename Ret>
    CaseBuilder& add(std::string name, Fn fn, const Ret& expected) {
        Case c;
        c.name = std::move(name);
        c.expected_str = fmt_value(expected);
        c.check = [expected](const std::string& actual) {
            return actual == fmt_value(expected);
        };
        // Store fn directly — no std::function conversion needed
        c.run = [fn = std::move(fn)]() {
            return fmt_value(fn());
        };
        cases_.push_back(std::move(c));
        return *this;
    }

    template <typename Ret>
    CaseBuilder& add_tol(std::string name, std::function<Ret()> fn,
                         const Ret& expected, double tol) {
        Case c;
        c.name = std::move(name);
        c.expected_str = fmt_value(expected);
        c.check = [expected, tol](const std::string& actual) {
            std::istringstream iss(actual);
            double actual_val = 0;
            iss >> actual_val;
            return std::fabs(actual_val - expected) <= tol;
        };
        c.run = [fn = std::move(fn)]() {
            return fmt_value(fn());
        };
        cases_.push_back(std::move(c));
        return *this;
    }

    void finish() {
        auto& sol = registry().at(algo_name_);
        if (is_bench_) {
            sol.bench_cases = std::move(cases_);
        } else {
            sol.cases = std::move(cases_);
        }
    }

private:
    std::string algo_name_;
    std::vector<Case> cases_;
    bool is_bench_;
};

/* ---------- Runner ---------- */

inline int run_all(const std::vector<std::string>& selected_names, bool bench) {
    auto& reg = registry();

    if (reg.empty()) {
        std::cout << "没有找到任何算法。请注册算法。\n";
        return 1;
    }

    std::vector<std::string> selected = selected_names;
    if (selected.empty()) {
        for (const auto& [name, _] : reg) selected.push_back(name);
    }

    for (const auto& name : selected_names) {
        if (reg.find(name) == reg.end()) {
            std::cerr << "未找到算法: " << name << "\n";
            std::cerr << "可用算法: ";
            bool first = true;
            for (const auto& [n, _] : reg) {
                if (!first) std::cerr << ", ";
                std::cerr << n;
                first = false;
            }
            std::cerr << "\n";
            return 1;
        }
    }

    int total_pass = 0, total_cases = 0;
    std::vector<std::string> failed;

    for (const auto& name : selected) {
        auto& sol = reg[name];
        std::cout << "== " << sol.name;
        if (!sol.desc.empty()) std::cout << " - " << sol.desc;
        std::cout << "\n";

        if (sol.cases.empty()) {
            std::cout << "  (无测试用例，跳过验证)\n";
        } else {
            int passed = 0;
            int total = static_cast<int>(sol.cases.size());
            auto start = std::chrono::steady_clock::now();

            for (auto& c : sol.cases) {
                total_cases++;
                std::string actual;
                try {
                    actual = c.run();
                } catch (const std::exception& e) {
                    std::cout << "  [FAIL] " << c.name
                              << ": 执行抛出异常 — " << e.what() << "\n";
                    continue;
                }

                bool ok = c.check(actual);
                if (ok) {
                    passed++;
                    std::cout << "  [PASS] " << c.name << "\n";
                } else {
                    std::cout << "  [FAIL] " << c.name << "\n";
                    std::cout << "         期望: " << c.expected_str << "\n";
                    std::cout << "         实际: " << actual << "\n";
                }
            }

            auto elapsed = std::chrono::steady_clock::now() - start;
            double ms =
                std::chrono::duration_cast<std::chrono::microseconds>(elapsed)
                        .count() /
                1000.0;
            total_pass += passed;
            std::cout << "  结果: " << passed << "/" << total << " 通过, 耗时 "
                      << std::fixed << std::setprecision(1) << ms << " ms\n";
            if (passed < total) failed.push_back(name);
        }

        if (bench) {
            if (sol.bench_cases.empty()) {
                std::cout << "  (无基准用例，跳过基准测试)\n";
            } else {
                for (auto& c : sol.bench_cases) {
                    try { c.run(); } catch (...) {}

                    int runs = 3;
                    double best_ms = 1e9;
                    for (int i = 0; i < runs; i++) {
                        auto t0 = std::chrono::steady_clock::now();
                        c.run();
                        auto t1 = std::chrono::steady_clock::now();
                        double ms = std::chrono::duration_cast<
                                        std::chrono::microseconds>(t1 - t0)
                                        .count() /
                                    1000.0;
                        if (ms < best_ms) best_ms = ms;
                    }
                    std::cout << "  [BENCH] " << c.name << ": 最快 "
                              << std::fixed << std::setprecision(3) << best_ms
                              << " ms (共 " << runs << " 次)\n";
                }
            }
        }

        std::cout << "\n";
    }

    if (total_cases) {
        std::cout << "汇总: " << total_pass << "/" << total_cases << " 通过\n";
        if (!failed.empty()) {
            std::cout << "失败算法: ";
            for (size_t i = 0; i < failed.size(); i++) {
                if (i) std::cout << ", ";
                std::cout << failed[i];
            }
            std::cout << "\n";
            return 1;
        }
    }
    return 0;
}

}  // namespace algoframe

#endif  // ALGOFRAME_HPP
