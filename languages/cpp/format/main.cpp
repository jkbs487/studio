#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <ctime>
#include <cstring>

// ============================================================================
// 1. Basic Formatting
// ============================================================================
void basicFormatting() {
    std::cout << "\n========== 1. Basic Formatting ==========\n";

    // Basic type formatting
    std::string s1 = std::format("Integer: {}, Float: {}, String: {}", 42, 3.14, "hello");
    std::cout << s1 << "\n";

    // Positional arguments
    std::string s2 = std::format("{2} {0} {1}", "a", "b", "c");
    std::cout << "Positional: " << s2 << "\n";

    // Named arguments (C++23, may not be supported)
    // std::string s3 = std::format("name={name}, age={age}", std::arg("name", "Tom"), std::arg("age", 20));

    // Escape braces
    std::cout << std::format("Escaped braces: {{}} ") << "\n";
}

// ============================================================================
// 2. Number Formatting
// ============================================================================
void numberFormatting() {
    std::cout << "\n========== 2. Number Formatting ==========\n";

    // Base conversion
    std::cout << std::format("Decimal: {}, Hex: {:x}, Binary: {:b}\n", 255, 255, 255);

    // Float precision
    double pi = 3.14159265358979;
    std::cout << std::format("Default: {:.5f}, Fixed: {:.6f}, Scientific: {:.2e}\n", pi, pi, pi);

    // Integer padding and alignment
    std::cout << std::format("Padding: {:05}, Right: {:>6}, Left: {:<6}\n", 42, 42, 42);
    std::cout << std::format("Center: {:^8}\n", 42);

    // Sign
    std::cout << std::format("Positive: {:+}, Negative: {:+}\n", 42, -42);
    std::cout << std::format("Space: {: }, negative: {: }\n", 42, -42);

    // Hex prefix
    std::cout << std::format("hex: {:#x}, HEX: {:#X}\n", 255, 255);
}

// ============================================================================
// 3. Bool and Pointer Formatting
// ============================================================================
void boolPointerFormatting() {
    std::cout << "\n========== 3. Bool and Pointer Formatting ==========\n";

    // Bool formatting
    bool flag = true;
    std::cout << std::format("Default: {}, true->1: {:d}, true->yes: {}\n", flag, flag, std::format("{}", flag));

    // Pointer formatting
    int x = 42;
    int* ptr = &x;
    std::cout << std::format("Pointer: {:p}\n", static_cast<void*>(ptr));
}

// ============================================================================
// 4. String Formatting
// ============================================================================
void stringFormatting() {
    std::cout << "\n========== 4. String Formatting ==========\n";

    std::string str = "Hello";

    // String truncation
    std::cout << std::format("Truncate: {:.3}\n", str);

    // String alignment and padding
    std::cout << std::format("Left: {:10}\n", str);
    std::cout << std::format("Right: {:>10}\n", str);
    std::cout << std::format("Center: {:^12}\n", str);
    std::cout << std::format("Fill*: {:*<12}\n", str);
}

// ============================================================================
// 5. chrono time formatting (C++20)
// ============================================================================
void chronoFormatting() {
    std::cout << "\n========== 5. chrono Time Formatting ==========\n";

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* local = std::localtime(&t);

    // C++20 std::format for tm not fully supported, use strftime
    char buf[64];

    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    std::cout << "Current time: " << buf << "\n";

    std::strftime(buf, sizeof(buf), "%Y/%m/%d", local);
    std::cout << "Date: " << buf << "\n";

    std::strftime(buf, sizeof(buf), "%H:%M:%S", local);
    std::cout << "Time: " << buf << "\n";

    std::cout << "Note: std::format for tm not fully supported in AppleClang\n";
}

// ============================================================================
// 6. Using std::format_to
// ============================================================================
void formatToDemo() {
    std::cout << "\n========== 6. std::format_to ==========\n";

    std::string buffer;
    buffer.resize(100);  // Pre-allocate space

    // Format to char array
    auto end = std::format_to(buffer.begin(), "Integer: {}, Float: {:.2f}", 100, 3.14159);
    buffer.resize(end - buffer.begin());

    std::cout << buffer << "\n";

    // Using back_inserter
    std::string result;
    std::format_to(std::back_inserter(result), "{}, {}, {}", 1, 2, 3);
    std::cout << "back_inserter: " << result << "\n";
}

// ============================================================================
// 7. Width and Precision
// ============================================================================
void widthPrecisionDemo() {
    std::cout << "\n========== 7. Width and Precision ==========\n";

    // Dynamic width
    int width = 10;
    std::cout << std::format("Dynamic width: {:{}} {:{}}\n", "hello", width, 42, width);

    // Dynamic precision
    double val = 3.14159265358979;
    int prec = 4;
    std::cout << std::format("Dynamic precision: {:.{}f}\n", val, prec);
}

// ============================================================================
// 8. Comparison with Old Methods
// ============================================================================
void comparisonWithOld() {
    std::cout << "\n========== 8. Comparison with Old Methods ==========\n";

    std::string name = "Tom";
    int age = 25;
    double score = 98.5;

    // Old: printf (unsafe)
    printf("printf: Name=%s, Age=%d, Score=%.1f\n", name.c_str(), age, score);

    // Old: ostringstream
    std::ostringstream oss;
    oss << "ostringstream: Name=" << name << ", Age=" << age << ", Score=" << score;
    std::cout << oss.str() << "\n";

    // New: std::format (type-safe)
    std::string result = std::format("std::format: Name={}, Age={}, Score={:.1f}", name, age, score);
    std::cout << result << "\n";

    // Performance comparison
    const int iterations = 10000;

    // printf performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        printf("%d %.2f %s\n", i, (double)i, "test");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto printfTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // std::format performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::string s = std::format("{} {:.2f} {}", i, (double)i, "test");
    }
    end = std::chrono::high_resolution_clock::now();
    auto formatTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "\nPerformance (" << iterations << " iterations):\n";
    std::cout << "printf: " << printfTime << " us\n";
    std::cout << "std::format: " << formatTime << " us\n";
    std::cout << "Note: std::format is type-safe and more readable, with slight performance overhead\n";
}

// ============================================================================
// 9. Combined Examples
// ============================================================================
void combinedDemo() {
    std::cout << "\n========== 9. Combined Examples ==========\n";

    // Table output
    std::cout << std::format("{:^30}\n", "Student Score Report");
    std::cout << std::format("{:<10} {:>6} {:>8}\n", "Name", "Math", "English");
    std::cout << std::format("{:-<10} {:-<6} {:-<8}\n", "", "", "");
    std::cout << std::format("{:<10} {:>6.1f} {:>8.1f}\n", "Alice", 95.5, 88.0);
    std::cout << std::format("{:<10} {:>6.1f} {:>8.1f}\n", "Bob", 87.3, 92.5);
    std::cout << std::format("{:<10} {:>6.1f} {:>8.1f}\n", "Charlie", 78.0, 85.5);

    // Formatted log (using strftime directly)
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* local = std::localtime(&t);

    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", local);

    std::cout << "\nFormatted log:\n";
    std::cout << "[" << timeBuf << "] [INFO] " << "Application started\n";
    std::cout << "[" << timeBuf << "] [WARN] " << "Memory usage high\n";
    std::cout << "[" << timeBuf << "] [ERROR] " << "Connection failed\n";
}

// ============================================================================
// Main
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "       C++20 std::format Library\n";
    std::cout << "========================================\n";

    basicFormatting();
    numberFormatting();
    boolPointerFormatting();
    stringFormatting();
    chronoFormatting();
    formatToDemo();
    widthPrecisionDemo();
    comparisonWithOld();
    combinedDemo();

    std::cout << "\n========================================\n";
    std::cout << "          Demo Complete\n";
    std::cout << "========================================\n";

    return 0;
}
