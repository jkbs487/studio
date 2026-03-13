// C++ 性能测试
// 编译: g++ -O2 -o benchmark_cpp main.cpp
// 运行: ./benchmark_cpp

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace chrono;

// 测试1: 斐波那契数列（递归）
long long fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 测试2: 质数筛选
int countPrimes(int n) {
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    int count = 0;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) count++;
    }
    return count;
}

// 测试3: 字符串拼接
string stringConcat(int n) {
    string result;
    for (int i = 0; i < n; i++) {
        result += to_string(i);
    }
    return result;
}

// 测试4: 数组排序
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    cout << "========== C++ 性能测试 ==========" << endl;
    cout << "编译优化: -O2" << endl;
    cout << "=================================" << endl;

    // 测试1: 斐波那契数列
    auto start = high_resolution_clock::now();
    long long fibResult = fibonacci(30);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "测试1 - 斐波那契(30): " << fibResult << " | 耗时: " << duration.count() << " ms" << endl;

    // 测试2: 质数筛选
    start = high_resolution_clock::now();
    int primeCount = countPrimes(100000);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "测试2 - 质数筛选(100000): " << primeCount << " 个 | 耗时: " << duration.count() << " ms" << endl;

    // 测试3: 字符串拼接
    start = high_resolution_clock::now();
    string strResult = stringConcat(10000);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "测试3 - 字符串拼接(10000次): " << strResult.length() << " 字符 | 耗时: " << duration.count() << " ms" << endl;

    // 测试4: 数组排序
    vector<int> arr(5000);
    for (int i = 0; i < 5000; i++) {
        arr[i] = 5000 - i;
    }
    start = high_resolution_clock::now();
    bubbleSort(arr);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "测试4 - 冒泡排序(5000元素): " << (arr[0] == 1 ? "正确" : "错误") << " | 耗时: " << duration.count() << " ms" << endl;

    cout << "=================================" << endl;
    cout << "C++ 测试完成" << endl;

    return 0;
}
