---
title: python-dataclasses
description: 探索 Python dataclasses：数据类装饰器、字段选项、比较方法、冻结
---

# python-dataclasses

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | python |
| 版本 | N/A |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `dataclasses` `data-class` `advanced` |

</div>

## 描述

探索 Python dataclasses：数据类装饰器、字段选项、比较方法、冻结

# 数据类 (dataclasses) `Python 3.7+`

本实验探索 Python `dataclasses` 的用法，大幅减少样板代码。

## 运行

```bash
python languages/python/dataclasses/main.py
```

## 内容

### 基础用法
- 自动生成 `__init__`, `__repr__`, `__eq__`
- 带默认值的字段

### 字段选项 (field)
- `field(default=...)` 默认值
- `field(default_factory=...)` 可变默认值
- `field(init=False)` 不参与初始化

### 排序比较 (order=True)
- 自动生成 `__lt__`, `__le__`, `__gt__`, `__ge__`

### 冻结对象 (frozen=True)
- 不可变对象，类似 namedtuple

### post_init 钩子
- 在 `__init__` 后执行自定义逻辑

### 实用模式
- 配置类、数据传输对象(DTO)
- 状态对象

### 转换
- `asdict()` 转换为字典

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码


## 源码

<details>
<summary>main.py</summary>

```python
#!/usr/bin/env python3
"""
Python dataclasses 实验

本实验探索 dataclasses 的各种用法，包括：
- 基础用法
- 字段选项 (field)
- 比较方法
- 冻结对象
- 继承
- 实用模式
"""

from dataclasses import dataclass, field, fields, asdict
from typing import List, Optional
import sys


def print_header(title: str):
    print(f"\n{'=' * 50}")
    print(f"  {title}")
    print('=' * 50)


# ============ 基础用法 ============

@dataclass
class User:
    name: str
    age: int
    email: str = ""  # 带默认值的字段必须在无默认值字段之后


def basic_usage():
    print_header("1. 基础用法")

    user = User(name="Alice", age=30)
    print(f"创建用户: {user}")
    print(f"姓名: {user.name}, 年龄: {user.age}")

    # 自动生成 __repr__
    print(f"repr: {repr(user)}")

    # 自动生成 __eq__
    user2 = User(name="Alice", age=30)
    print(f"user == user2: {user == user2}")


# ============ 字段选项 ============

@dataclass
class Product:
    name: str
    price: float
    quantity: int = 0
    tags: List[str] = field(default_factory=list)  # 使用 factory 创建可变默认


def field_options():
    print_header("2. 字段选项")

    product = Product(name="Laptop", price=999.99, quantity=5)
    print(f"产品: {product}")

    # 添加标签
    product.tags.append("electronics")
    product.tags.append("sale")
    print(f"标签: {product.tags}")

    # 查看所有字段
    print("\n所有字段:")
    for f in fields(Product):
        print(f"  - {f.name}: {f.type}, default={f.default}")


# ============ 排序比较 ============

@dataclass(order=True)
class Point:
    x: int
    y: int


def comparison():
    print_header("3. 排序比较 (order=True)")

    points = [Point(1, 2), Point(3, 1), Point(1, 1)]
    print(f"排序前: {points}")
    print(f"排序后: {sorted(points)}")
    print(f"Point(1,1) < Point(3,1): {Point(1, 1) < Point(3, 1)}")


# ============ 冻结对象 ============

@dataclass(frozen=True)
class FrozenConfig:
    name: str
    version: str = "1.0"


def frozen_object():
    print_header("4. 冻结对象 (frozen=True)")

    config = FrozenConfig(name="app")
    print(f"配置: {config}")

    try:
        config.name = "new_app"
    except Exception as e:
        print(f"修改冻结对象时抛出异常: {type(e).__name__}")


# ============ post_init ============

@dataclass
class Rectangle:
    width: float
    height: float
    area: float = field(init=False)  # 不在 __init__ 中初始化

    def __post_init__(self):
        self.area = self.width * self.height


def post_init():
    print_header("5. post_init 钩子")

    rect = Rectangle(width=5, height=3)
    print(f"矩形: {rect}")
    print(f"面积: {rect.area}")


# ============ 实用模式 ============

@dataclass
class DatabaseConfig:
    host: str
    port: int
    database: str
    username: str = "root"
    password: str = ""
    pool_size: int = 10
    timeout: int = 30

    def get_url(self) -> str:
        return f"postgresql://{self.username}:{self.password}@{self.host}:{self.port}/{self.database}"


@dataclass
class Response:
    status_code: int
    data: Optional[dict] = None
    error: Optional[str] = None
    headers: dict = field(default_factory=dict)

    def is_success(self) -> bool:
        return 200 <= self.status_code < 300


def practical_patterns():
    print_header("6. 实用模式")

    # 数据库配置
    db_config = DatabaseConfig(
        host="localhost",
        port=5432,
        database="mydb"
    )
    print(f"数据库 URL: {db_config.get_url()}")

    # API 响应
    success_response = Response(status_code=200, data={"message": "ok"})
    error_response = Response(status_code=404, error="Not found")

    print(f"成功响应: {success_response.is_success()}")
    print(f"错误响应: {success_response.is_success()}")


# ============ 转换为字典 ============

@dataclass
class Person:
    name: str
    age: int
    city: str = "Unknown"


def conversion():
    print_header("7. 转换")

    person = Person(name="Bob", age=25, city="Beijing")
    # 转换为字典
    person_dict = asdict(person)
    print(f"字典: {person_dict}")


def main():
    print("Python dataclasses 实验")
    print(f"Python 版本: {sys.version}")

    basic_usage()
    field_options()
    comparison()
    frozen_object()
    post_init()
    practical_patterns()
    conversion()

    print("\n" + "=" * 50)
    print("  实验完成!")
    print("=" * 50)


if __name__ == "__main__":
    main()

```

</details>



<style>
.experiment-meta {
  margin: 1rem 0;
}
.experiment-meta table {
  margin: 0;
}
</style>
