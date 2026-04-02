#include <iostream>
#include <span>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstring>

// ============================================================================
// 1. Basic span Creation
// ============================================================================
void basicSpanCreation() {
    std::cout << "\n========== 1. Basic span Creation ==========\n";

    // From C-style array
    int arr[] = {1, 2, 3, 4, 5};
    std::span<int> s1(arr);
    std::cout << "From C-array: ";
    for (int v : s1) std::cout << v << " ";
    std::cout << "\n";

    // From std::vector
    std::vector<int> vec = {10, 20, 30, 40, 50};
    std::span<int> s2(vec);
    std::cout << "From vector: ";
    for (int v : s2) std::cout << v << " ";
    std::cout << "\n";

    // From std::array
    std::array<int, 5> arr2 = {100, 200, 300, 400, 500};
    std::span<int> s3(arr2);
    std::cout << "From std::array: ";
    for (int v : s3) std::cout << v << " ";
    std::cout << "\n";

    // Empty span
    std::span<int> empty;
    std::cout << "Empty span size: " << empty.size() << "\n";
}

// ============================================================================
// 2. Span Properties
// ============================================================================
void spanProperties() {
    std::cout << "\n========== 2. Span Properties ==========\n";

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::span<int> s(arr);

    std::cout << "Size: " << s.size() << "\n";
    std::cout << "Size in bytes: " << s.size_bytes() << "\n";
    std::cout << "Is empty: " << (s.empty() ? "yes" : "no") << "\n";
    std::cout << "First element: " << s.front() << "\n";
    std::cout << "Last element: " << s.back() << "\n";
    std::cout << "Element at index 2: " << s[2] << "\n";
    std::cout << "Element at index 2 (data()): " << s.data()[2] << "\n";
}

// ============================================================================
// 3. Subspans
// ============================================================================
void subspanDemo() {
    std::cout << "\n========== 3. Subspans ==========\n";

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::span<int> s(arr);

    // first(n) - first n elements
    std::span<int> first3 = s.first(3);
    std::cout << "First 3: ";
    for (int v : first3) std::cout << v << " ";
    std::cout << "\n";

    // last(n) - last n elements
    std::span<int> last3 = s.last(3);
    std::cout << "Last 3: ";
    for (int v : last3) std::cout << v << " ";
    std::cout << "\n";

    // subspan(offset, count) - middle elements
    std::span<int> middle = s.subspan(3, 4);
    std::cout << "Subspan(3,4): ";
    for (int v : middle) std::cout << v << " ";
    std::cout << "\n";

    // subspan(offset) - from offset to end
    std::span<int> from5 = s.subspan(5);
    std::cout << "Subspan(5): ";
    for (int v : from5) std::cout << v << " ";
    std::cout << "\n";
}

// ============================================================================
// 4. Iterator Support
// ============================================================================
void iteratorSupport() {
    std::cout << "\n========== 4. Iterator Support ==========\n";

    int arr[] = {10, 20, 30, 40, 50};
    std::span<int> s(arr);

    // Forward iteration
    std::cout << "Forward: ";
    for (auto it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Reverse iteration
    std::cout << "Reverse: ";
    for (auto it = s.rbegin(); it != s.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Range-based for
    std::cout << "Range-based for: ";
    for (int& v : s) {
        v *= 2;  // modify through span
    }
    for (int v : s) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

// ============================================================================
// 5. Const Span
// ============================================================================
void constSpanDemo() {
    std::cout << "\n========== 5. Const Span ==========\n";

    const std::vector<int> vec = {1, 2, 3, 4, 5};
    std::span<const int> const_span(vec);

    std::cout << "Const span: ";
    for (int v : const_span) {
        std::cout << v << " ";
        // v = 10;  // Error: cannot modify through const span
    }
    std::cout << "\n";

    // Conversion to const span
    std::vector<int> mutable_vec = {10, 20, 30};
    std::span<int> mutable_span(mutable_vec);
    std::span<const int> converted_span = mutable_span;  // implicit conversion
    std::cout << "Converted to const: ";
    for (int v : converted_span) std::cout << v << " ";
    std::cout << "\n";
}

// ============================================================================
// 6. Static Extent Span
// ============================================================================
void staticExtentSpan() {
    std::cout << "\n========== 6. Static Extent Span ==========\n";

    std::array<int, 5> arr = {1, 2, 3, 4, 5};

    // Dynamic extent (size known at runtime)
    std::span<int> dynamic_span(arr);
    std::cout << "Dynamic extent: size=" << dynamic_span.size() << "\n";

    // Static extent (size known at compile time)
    std::span<int, 5> static_span(arr);
    std::cout << "Static extent: size=" << static_span.size() << "\n";

    // Compile-time size benefits
    constexpr size_t static_size = std::span<int, 5>::extent;
    std::cout << "Compile-time extent: " << static_size << "\n";
}

// ============================================================================
// 7. Span as Function Parameter
// ============================================================================
void processVector(std::span<int> data) {
    std::cout << "Processing span of size: " << data.size() << "\n";
    std::cout << "Sum: " << std::accumulate(data.begin(), data.end(), 0) << "\n";
}

// More efficient than taking vector by const reference
void printData(std::span<const int> data) {
    for (int v : data) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

// Function that modifies data through span
void doubleValues(std::span<int> data) {
    for (int& v : data) {
        v *= 2;
    }
}

void functionParameterDemo() {
    std::cout << "\n========== 7. Span as Function Parameter ==========\n";

    // Can accept various container types
    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {10, 20, 30, 40, 50};
    std::array<int, 5> stdarr = {100, 200, 300, 400, 500};

    std::cout << "C-array:\n";
    processVector(arr);

    std::cout << "std::vector:\n";
    processVector(vec);

    std::cout << "std::array:\n";
    processVector(stdarr);

    // Read-only span
    std::cout << "Read-only span: ";
    printData(vec);

    // Modifying through span
    std::vector<int> mutable_vec = {1, 2, 3, 4, 5};
    std::cout << "Before: ";
    printData(mutable_vec);
    doubleValues(mutable_vec);
    std::cout << "After double: ";
    printData(mutable_vec);
}

// ============================================================================
// 8. Span with Algorithms
// ============================================================================
void algorithmDemo() {
    std::cout << "\n========== 8. Span with Algorithms ==========\n";

    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    std::span<int> s(vec);

    // Sort
    std::sort(s.begin(), s.end());
    std::cout << "Sorted: ";
    for (int v : s) std::cout << v << " ";
    std::cout << "\n";

    // Find
    auto it = std::find(s.begin(), s.end(), 5);
    if (it != s.end()) {
        std::cout << "Found 5 at index: " << (it - s.begin()) << "\n";
    }

    // Count
    int count = std::count(s.begin(), s.end(), 1);
    std::cout << "Count of 1: " << count << "\n";

    // Accumulate
    int sum = std::accumulate(s.begin(), s.end(), 0);
    std::cout << "Sum: " << sum << "\n";

    // Transform
    std::transform(s.begin(), s.end(), s.begin(),
                   [](int x) { return x * x; });
    std::cout << "Squared: ";
    for (int v : s) std::cout << v << " ";
    std::cout << "\n";
}

// ============================================================================
// 9. Span for String Views
// ============================================================================
void stringSpanDemo() {
    std::cout << "\n========== 9. Span for String Views ==========\n";

    std::string str = "Hello, World!";
    std::span<char> char_span(str.data(), str.size());

    std::cout << "Character span: ";
    for (char c : char_span) std::cout << c;
    std::cout << "\n";

    // Substring view
    std::span<char> substring = char_span.subspan(7, 5);
    std::cout << "Substring (7,5): ";
    for (char c : substring) std::cout << c;
    std::cout << "\n";

    // Const char span
    std::string const_str = "Const string";
    std::span<const char> const_span(const_str);
    std::cout << "Const span: ";
    for (char c : const_span) std::cout << c;
    std::cout << "\n";
}

// ============================================================================
// 10. Zero-Copy Operations
// ============================================================================
void zeroCopyDemo() {
    std::cout << "\n========== 10. Zero-Copy Operations ==========\n";

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Processing functions work on spans without copying data
    auto process = [](std::span<const int> s, std::string_view name) {
        std::cout << name << ": sum=" << std::accumulate(s.begin(), s.end(), 0);
        std::cout << ", avg=" << (std::accumulate(s.begin(), s.end(), 0.0) / s.size()) << "\n";
    };

    process(data, "Full data");
    process(std::span<int>(data).first(5), "First half");
    process(std::span<int>(data).last(5), "Second half");
    process(std::span<int>(data).subspan(2, 6), "Middle");
}

// ============================================================================
// 11. Span with Raw Pointers
// ============================================================================
void rawPointerDemo() {
    std::cout << "\n========== 11. Span with Raw Pointers ==========\n";

    // C-style allocation
    int* ptr = new int[5]{10, 20, 30, 40, 50};
    std::span<int> s(ptr, 5);

    std::cout << "From raw pointer: ";
    for (int v : s) std::cout << v << " ";
    std::cout << "\n";

    // Safe access with bounds checking
    if (!s.empty()) {
        std::cout << "First: " << s.front() << "\n";
        std::cout << "Last: " << s.back() << "\n";
    }

    delete[] ptr;
}

// ============================================================================
// 12. Span Type Deduction
// ============================================================================
template<typename T>
void processGeneric(std::span<T> data) {
    std::cout << "Processing span of " << typeid(T).name() << ", size: " << data.size() << "\n";
    for (auto& v : data) {
        v = v + 1;
    }
}

void typeDeductionDemo() {
    std::cout << "\n========== 12. Span Type Deduction ==========\n";

    std::vector<int> int_vec = {1, 2, 3};
    processGeneric(std::span(int_vec));

    std::vector<double> double_vec = {1.1, 2.2, 3.3};
    processGeneric(std::span(double_vec));

    std::cout << "After processing: ";
    for (int v : int_vec) std::cout << v << " ";
    std::cout << "\n";
}

// ============================================================================
// 13. Span vs Vector Performance
// ============================================================================
void performanceComparison() {
    std::cout << "\n========== 13. Performance Comparison ==========\n";

    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) {
        data[i] = i;
    }

    // Function taking vector by const reference
    auto processVectorRef = [](const std::vector<int>& vec) {
        return std::accumulate(vec.begin(), vec.end(), 0);
    };

    // Function taking span
    auto processSpan = [](std::span<const int> s) {
        return std::accumulate(s.begin(), s.end(), 0);
    };

    const int iterations = 10000;

    // Vector reference performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        processVectorRef(data);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto vector_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Span performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        processSpan(data);
    }
    end = std::chrono::high_resolution_clock::now();
    auto span_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Vector reference: " << vector_time << " us\n";
    std::cout << "Span: " << span_time << " us\n";
    std::cout << "Span is " << ((vector_time > span_time) ? "faster" : "similar") << " or more flexible\n";
}

// ============================================================================
// Main
// ============================================================================
int main() {
    std::cout << "========================================\n";
    std::cout << "       C++20 std::span Library\n";
    std::cout << "========================================\n";

    basicSpanCreation();
    spanProperties();
    subspanDemo();
    iteratorSupport();
    constSpanDemo();
    staticExtentSpan();
    functionParameterDemo();
    algorithmDemo();
    stringSpanDemo();
    zeroCopyDemo();
    rawPointerDemo();
    typeDeductionDemo();
    performanceComparison();

    std::cout << "\n========================================\n";
    std::cout << "          Demo Complete\n";
    std::cout << "========================================\n";

    return 0;
}
