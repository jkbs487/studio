/**
 * C++ 算法验证入口。
 *
 * 用法：
 *   ./main                  # 验证所有算法
 *   ./main two_sum          # 只验证指定算法
 *   ./main two_sum --bench  # 验证并附加基准测试
 *   ./main --bench          # 全部验证并基准测试
 */

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "algoframe.hpp"

// 声明各 solution 文件的注册函数
void register_example_algorithms();

int main(int argc, char* argv[]) {
    std::vector<std::string> names;
    bool bench = false;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--bench") == 0) {
            bench = true;
        } else {
            names.emplace_back(argv[i]);
        }
    }

    // 注册所有算法
    register_example_algorithms();

    return algoframe::run_all(names, bench);
}
