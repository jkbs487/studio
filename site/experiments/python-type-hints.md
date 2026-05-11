---
title: python-type-hints
description: 探索 Python 类型提示系统：基础注解、typing 模块、泛型、Protocol
---

# python-type-hints

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | python |
| 版本 | N/A |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `type-hints` `typing` `advanced` |

</div>

## 描述

探索 Python 类型提示系统：基础注解、typing 模块、泛型、Protocol

# Python 类型提示 (Type Hints) `Python 3.9+`

探索 Python 类型提示系统的各种特性。

## 运行

```bash
python languages/python/type-hints/main.py
```

## 要求

- Python 3.9+ (部分特性需要更高版本，详见下文)

## 内容

### 基础类型注解
- 基础类型: `str`, `int`, `float`, `bool`
- 函数参数和返回值注解

### 泛型 (Generic)
- `TypeVar` 基础用法
- `Generic[K, V]` 多类型参数

### Protocol (结构化类型)
- `@runtime_checkable` 运行时检查
- 结构化子类型

### Union 和 Literal
- `Union[X, Y]` 多类型
- `Literal['a', 'b']` 精确值

### Final 和 TypeAlias
- `Final` 不可变变量
- `TypeAlias` 类型别名

### Self 类型 (Python 3.11+)
- 链式调用中的自引用

### Callable 和高阶函数
- `Callable[[int], int]` 函数类型
- `ParamSpec` 参数规格 (Python 3.10+)

### TypedDict
- 必填和可选字段
- `total=False` 所有字段可选

### Annotated
- 带元数据的类型注解

### TypeGuard
- 类型守卫缩小类型范围

### NoReturn
- 永不返回的函数

### 泛型边界
- `TypeVar('T', bound=Animal)`

### 内置泛型 (Python 3.9+)
- `list[int]`, `dict[str, int]`

### @overload 重载
- 函数重载实现

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
Python 类型提示实验

本实验探索 Python 类型提示系统的各种特性。
"""

import sys
from typing import (
    TypeVar, Generic, Protocol, Optional, Union, Literal, Callable,
    overload, TypedDict, runtime_checkable, Final, TypeAlias,
    TypeGuard, Annotated, NoReturn, ParamSpec, Concatenate
)


def print_header(title: str):
    print(f"\n{'=' * 50}")
    print(f"  {title}")
    print('=' * 50)


# ============ 1. 基础类型注解 ============

def greet(name: str, age: int = 18) -> str:
    return f"你好, {name}! 你今年 {age} 岁。"


def basic_types():
    print_header("1. 基础类型注解")

    # 基础类型
    name: str = "张三"
    age: int = 25
    height: float = 1.75
    is_active: bool = True

    print(f"姓名: {name}, 年龄: {age}, 身高: {height}, 激活: {is_active}")
    print(f"greet 结果: {greet(name, age)}")


# ============ 2. 泛型 (Generic) ============

T = TypeVar('T')
K = TypeVar('K')
V = TypeVar('V')


class Box(Generic[T]):
    def __init__(self, value: T) -> None:
        self._value = value

    def get(self) -> T:
        return self._value

    def set(self, value: T) -> None:
        self._value = value


class DictStore(Generic[K, V]):
    def __init__(self) -> None:
        self._data: dict[K, V] = {}

    def put(self, key: K, value: V) -> None:
        self._data[key] = value

    def get(self, key: K) -> Optional[V]:
        return self._data.get(key)


def generics():
    print_header("2. 泛型 (Generic)")

    # 基础泛型
    int_box = Box(42)
    str_box = Box("hello")
    print(f"Int Box: {int_box.get()}")
    print(f"Str Box: {str_box.get()}")

    # 字典泛型
    store: DictStore[str, int] = DictStore()
    store.put("score", 100)
    print(f"字典存储: {store.get('score')}")


# ============ 3. Protocol (结构化类型) ============

@runtime_checkable
class Drawable(Protocol):
    def draw(self) -> str: ...


class Circle:
    def __init__(self, radius: float) -> None:
        self.radius = radius

    def draw(self) -> str:
        return f"圆: 半径{self.radius}"


class Rectangle:
    def __init__(self, width: float, height: float) -> None:
        self.width = width
        self.height = height

    def draw(self) -> str:
        return f"矩形: {self.width}x{self.height}"


def draw_all(items: list[Drawable]) -> list[str]:
    return [item.draw() for item in items]


def protocol_demo():
    print_header("3. Protocol (结构化类型)")

    circle = Circle(5.0)
    rect = Rectangle(3, 4)

    # Protocol 允许静态类型检查
    print(f"Circle 是 Drawable: {isinstance(circle, Drawable)}")
    print(f"Rectangle 是 Drawable: {isinstance(rect, Drawable)}")

    results = draw_all([circle, rect])
    for r in results:
        print(f"  {r}")


# ============ 4. Union 和 Literal ============

Mode = Literal['fast', 'normal', 'slow']
Status = Literal['pending', 'processing', 'done', 'failed']


def run_task(mode: Mode) -> str:
    return f"模式: {mode}"


# Union: 多种类型之一
Number = Union[int, float]


def process(value: Number) -> Number:
    return value * 2


def union_literal():
    print_header("4. Union 和 Literal")

    # Literal: 精确值
    print(f"Literal: {run_task('fast')}")

    # Union: 多种类型
    result: Number = process(10)
    print(f"Union[int, float]: {result}")
    result = process(3.14)
    print(f"Union[int, float]: {result}")


# ============ 5. Final 和 TypeAlias ============

# Final: 不可变
MAX_SIZE: Final[int] = 100
DEFAULT_NAME: Final[str] = "Unnamed"

# TypeAlias: 类型别名 (Python 3.10+)
Vector2D: TypeAlias = tuple[float, float]
Matrix: TypeAlias = list[list[float]]


def process_vector(v: Vector2D) -> float:
    return (v[0] ** 2 + v[1] ** 2) ** 0.5


def final_typealias():
    print_header("5. Final 和 TypeAlias")

    # Final 变量
    print(f"MAX_SIZE: {MAX_SIZE}")
    # MAX_SIZE = 200  # TypeError: Cannot assign to final name 'MAX_SIZE'

    # TypeAlias
    vec: Vector2D = (3.0, 4.0)
    print(f"Vector2D: {vec}, 长度: {process_vector(vec)}")


# ============ 6. Self 类型 (Python 3.11+) ============

if sys.version_info >= (3, 11):
    from typing import Self

    class Builder:
        def __init__(self) -> None:
            self._value = ""

        def set_value(self, value: str) -> Self:
            self._value = value
            return self

        def build(self) -> str:
            return self._value

    def self_type():
        print_header("6. Self 类型 (Python 3.11+)")

        result = Builder().set_value("hello").set_value("world").build()
        print(f"链式调用结果: {result}")
else:
    def self_type():
        print_header("6. Self 类型")
        print("需要 Python 3.11+ 才能使用")


# ============ 7. Callable 和高阶函数 ============

def apply(value: int, op: Callable[[int], int]) -> int:
    return op(value)


def make_adder(n: int) -> Callable[[int], int]:
    """返回闭包: 加法器"""
    def adder(x: int) -> int:
        return x + n
    return adder


# ParamSpec: 捕获函数参数 (Python 3.10+)
if sys.version_info >= (3, 10):
    P = ParamSpec('P')

    # 简化示例: 使用 ParamSpec 保留函数签名
    def add_prefix(func: Callable[P, int], prefix: str) -> Callable[P, str]:
        """为函数结果添加前缀"""
        def wrapper(*args: P.args, **kwargs: P.kwargs) -> str:
            result = func(*args, **kwargs)
            return f"{prefix}: {result}"
        return wrapper


def callable_demo():
    print_header("7. Callable 和高阶函数")

    # 使用 Callable
    result = apply(10, lambda x: x * 2)
    print(f"apply(10, lambda x: x * 2): {result}")

    # 闭包工厂
    add5 = make_adder(5)
    print(f"make_adder(5)(10): {add5(10)}")

    # 使用 ParamSpec
    if sys.version_info >= (3, 10):
        def multiply(x: int, y: int) -> int:
            return x * y
        wrapped = add_prefix(multiply, "结果")
        print(f"add_prefix(multiply, '结果')(2, 3): {wrapped(2, 3)}")


# ============ 8. TypedDict ============

class UserDict(TypedDict):
    name: str
    age: int
    email: Optional[str]


# Total=False: 所有字段都是可选的
class Config(TypedDict, total=False):
    debug: bool
    max_connections: int
    timeout: float


def typeddict_demo():
    print_header("8. TypedDict")

    # 必填字段
    user: UserDict = {"name": "张三", "age": 25}
    print(f"UserDict: {user}")

    # 可选字段
    user_with_email: UserDict = {"name": "李四", "age": 30, "email": "li@example.com"}
    print(f"UserDict (with email): {user_with_email}")

    # total=False
    config: Config = {"debug": True}
    print(f"Config: {config}")


# ============ 9. Annotated (带元数据的类型) ============

# Annotated[类型, 元数据]
Range = Annotated[int, "must be positive"]
ValidatedString = Annotated[str, "min_length: 3", "max_length: 10"]


def process_range(value: Range) -> int:
    if value <= 0:
        raise ValueError("Value must be positive")
    return value * 2


def annotated_demo():
    print_header("9. Annotated (带元数据的类型)")

    result = process_range(10)
    print(f"process_range(10): {result}")


# ============ 10. TypeGuard ============

def is_string_list(val: list[object]) -> TypeGuard[list[str]]:
    """类型守卫: 缩小类型范围"""
    return all(isinstance(x, str) for x in val)


def process_list(items: list[object]) -> None:
    if is_string_list(items):
        # 在这个分支中,items 被识别为 list[str]
        print(f"字符串列表: {', '.join(items)}")
    else:
        print("混合类型列表")


def typeguard_demo():
    print_header("10. TypeGuard")

    process_list(["a", "b", "c"])
    process_list([1, 2, "three"])


# ============ 11. NoReturn ============

# NoReturn: 永不返回的函数 (如异常、无限循环)
def fatal_error(message: str) -> NoReturn:
    raise SystemExit(f"Fatal: {message}")


def noreturn_demo():
    print_header("11. NoReturn")

    try:
        # fatal_error("test")  # 取消注释会导致程序退出
        print("NoReturn 函数用于永不返回的函数 (如 sys.exit)")
    except SystemExit as e:
        print(f"捕获退出: {e}")


# ============ 12. 泛型边界 ============

class Animal:
    def speak(self) -> str:
        return "..."


class Dog(Animal):
    def speak(self) -> str:
        return "汪!"


class Cat(Animal):
    def speak(self) -> str:
        return "喵!"


# TypeVar 绑定: T 必须是 Animal 的子类
AnimalT = TypeVar('AnimalT', bound=Animal)


def make_speak(animal: AnimalT) -> AnimalT:
    print(f"动物说: {animal.speak()}")
    return animal


def generic_bound():
    print_header("12. 泛型边界 (bound)")

    dog = make_speak(Dog())
    cat = make_speak(Cat())


# ============ 13. 内置泛型 (Python 3.9+) ============

if sys.version_info >= (3, 9):
    def builtin_generics():
        print_header("13. 内置泛型 (Python 3.9+)")

        # list[int], dict[str, int] (直接使用,无需 typing.List)
        numbers: list[int] = [1, 2, 3]
        scores: dict[str, int] = {"语文": 90, "数学": 95}

        print(f"list[int]: {numbers}")
        print(f"dict[str, int]: {scores}")
else:
    def builtin_generics():
        print_header("13. 内置泛型")
        print("需要 Python 3.9+ 才能使用内置泛型语法")


# ============ 14. overload 重载 ============

@overload
def process(data: int) -> str: ...


@overload
def process(data: str) -> int: ...


@overload
def process(data: list[int]) -> int: ...


def process(data):
    if isinstance(data, int):
        return f"数字: {data}"
    elif isinstance(data, str):
        return len(data)
    elif isinstance(data, list):
        return sum(data)
    return 0


def overload_demo():
    print_header("14. @overload 重载")

    print(f"process(42): {process(42)}")
    print(f"process('hello'): {process('hello')}")
    print(f"process([1,2,3]): {process([1, 2, 3])}")


# ============ 15. 实用示例 ============

from dataclasses import dataclass


@dataclass
class User:
    id: int
    name: str
    roles: list[str]


class UserService:
    def __init__(self) -> None:
        self._users: dict[int, User] = {}

    def create(self, name: str, roles: list[str] | None = None) -> User:
        user_id = len(self._users) + 1
        user = User(id=user_id, name=name, roles=roles or ["user"])
        self._users[user_id] = user
        return user

    def find_by_role(self, role: str) -> list[User]:
        return [u for u in self._users.values() if role in u.roles]

    def find_by_id(self, user_id: int) -> User | None:
        return self._users.get(user_id)


def practical_demo():
    print_header("15. 实用示例")

    service = UserService()

    # 创建用户
    user1 = service.create("张三", ["admin", "editor"])
    user2 = service.create("李四", ["user"])
    user3 = service.create("王五")

    print(f"创建用户: {user1}, {user2}, {user3}")

    # 查找
    admins = service.find_by_role("admin")
    print(f"管理员: {[u.name for u in admins]}")

    # ID 查找
    found = service.find_by_id(2)
    print(f"ID=2 的用户: {found}")


def main():
    print("=" * 50)
    print("Python 类型提示实验")
    print(f"Python 版本: {sys.version}")
    print("=" * 50)

    basic_types()
    generics()
    protocol_demo()
    union_literal()
    final_typealias()
    self_type()
    callable_demo()
    typeddict_demo()
    annotated_demo()
    typeguard_demo()
    noreturn_demo()
    generic_bound()
    builtin_generics()
    overload_demo()
    practical_demo()

    print("\n" + "=" * 50)
    print("实验完成!")
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
