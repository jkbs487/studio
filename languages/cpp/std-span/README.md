# std::span 视图容器 (C++20)

C++20 引入了 `<span>` 库，提供了一个轻量级的、非拥有的连续序列视图，可以安全地引用各种容器类型。

## 运行

```bash
# 使用 CMake
cd languages/cpp
cmake -B build
cmake --build build
./build/std-span/std-span

# 或直接编译
g++ -std=c++20 -o std-span std-span/main.cpp
./std-span
```

## 内容

- **基础创建**: 从 C 数组、vector、array 创建 span
- **span 属性**: size()、size_bytes()、empty()、front()、back()、data()
- **子视图**: first()、last()、subspan()
- **迭代器支持**: begin/end、rbegin/rend、范围 for 循环
- **const span**: 只读视图，不能修改数据
- **静态扩展**: 编译时已知大小的 span
- **函数参数**: 接受多种容器类型的统一接口
- **算法集成**: 与 STL 算法配合使用
- **字符串视图**: 字符串的字符级操作
- **零拷贝操作**: 避免数据拷贝的高效处理
- **原始指针**: 与 C 风格指针的互操作
- **类型推导**: 模板函数中的类型推导
- **性能对比**: 与 vector 引用的性能比较

## 特性

- **零开销**: span 只是视图，不拥有数据
- **类型安全**: 编译时检查，防止越界
- **通用性**: 可以接受任何连续内存容器
- **高效性**: 避免数据拷贝，提升性能
- **灵活性**: 支持静态和动态大小

## 使用场景

### 1. 函数参数统一接口

```cpp
// 不需要为不同容器重载
void process(std::span<int> data);

int arr[] = {1, 2, 3};
std::vector<int> vec = {1, 2, 3};
std::array<int, 3> arr2 = {1, 2, 3};

process(arr);    // C 数组
process(vec);    // vector
process(arr2);   // array
```

### 2. 零拷贝数据处理

```cpp
void processChunk(std::span<const int> chunk) {
    // 处理数据块，无需拷贝
}

std::vector<int> data = /* ... */;
processChunk(data);              // 处理全部
processSpan(data.first(100));    // 处理前100个
processSpan(data.last(50));      // 处理后50个
```

### 3. 与 STL 算法配合

```cpp
std::vector<int> vec = {3, 1, 4, 1, 5};
std::span<int> s(vec);

std::sort(s.begin(), s.end());
int sum = std::accumulate(s.begin(), s.end(), 0);
```

## 注意事项

- span 不拥有数据，被引用的容器必须比 span 活得更久
- 避免返回临时对象的 span
- 修改 span 会修改原始数据（除非是 const span）
- 静态扩展的 span 在编译时检查大小
