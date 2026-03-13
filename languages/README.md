# 语言特性研究

此目录用于研究各种编程语言的特性。

## 目录组织

按语言名称组织，每个语言目录下按主题划分子目录：

```
languages/
├── python/              # Python 实验
│   ├── decorators/      # 装饰器
│   ├── async/           # 异步编程
│   ├── metaclass/       # 元类
│   ├── generators/      # 生成器
│   ├── type-hints/      # 类型提示
│   ├── pattern-matching/# 模式匹配
│   ├── dataclasses/     # 数据类
│   └── exception-groups/# 异常组
├── cpp/                 # C++ 实验
│   ├── templates/       # 模板
│   ├── smart-pointers/ # 智能指针
│   ├── lambda/          # Lambda 表达式
│   ├── move-semantics/  # 移动语义
│   ├── concurrency/     # 并发编程
│   └── ranges/          # Ranges (C++20)
├── go/                  # Go 实验
│   └── goroutines/     # 并发编程
└── ...
```

## 添加新语言研究

1. 创建语言目录（使用小写英文名）
2. 按主题创建子目录
3. 每个示例文件添加注释说明
