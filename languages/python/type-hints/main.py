#!/usr/bin/env python3
"""Python 类型提示实验"""

from typing import TypeVar, Generic, Protocol, Optional, Union, Literal, Callable, overload, TypedDict, runtime_checkable
from dataclasses import dataclass

# 1. 基础类型注解
def greet(name: str, age: int = 18) -> str:
    return f"你好, {name}! 你今年 {age} 岁。"

# 2. 泛型
T = TypeVar('T')

class Box(Generic[T]):
    def __init__(self, value: T) -> None:
        self._value = value
    def get(self) -> T:
        return self._value

# 3. Protocol
@runtime_checkable
class Drawable(Protocol):
    def draw(self) -> str: ...

class Circle:
    def __init__(self, radius: float) -> None:
        self.radius = radius
    def draw(self) -> str:
        return f"圆: 半径{self.radius}"

# 4. Union 和 Literal
Mode = Literal['fast', 'normal', 'slow']

def run_task(mode: Mode) -> str:
    return f"模式: {mode}"

# 5. Callable 和重载
def apply(value: int, op: Callable[[int], int]) -> int:
    return op(value)

@overload
def process(data: int) -> str: ...
@overload
def process(data: str) -> int: ...

def process(data):
    return f"数字: {data}" if isinstance(data, int) else len(data)

# 6. TypedDict
class Person(TypedDict):
    name: str
    age: int

# 7. 实际应用
@dataclass
class User:
    id: int
    name: str
    roles: list

class UserService:
    def __init__(self) -> None:
        self._users: dict = {}
    def create(self, name: str, roles: list = None) -> User:
        user_id = len(self._users) + 1
        user = User(id=user_id, name=name, roles=roles or ["user"])
        self._users[user_id] = user
        return user
    def find_by_role(self, role: str) -> list:
        return [u for u in self._users.values() if role in u.roles]

def main():
    print("=" * 50)
    print("Python 类型提示实验")
    print("=" * 50)
    
    print("\n1. 基础注解:", greet("张三", 25))
    print("2. 泛型:", Box(42).get())
    print("3. Protocol:", Circle(5.0).draw())
    print("4. Literal:", run_task('fast'))
    print("5. Callable:", apply(10, lambda x: x * 2))
    print("6. TypedDict:", Person(name="王五", age=30))
    
    print("\n7. 用户服务:")
    service = UserService()
    user = service.create("张三", ["admin"])
    print(f"   用户: {user}")
    print(f"   查找 admin: {[u.name for u in service.find_by_role('admin')]}")
    
    print("\n" + "=" * 50)
    print("完成!")

if __name__ == "__main__":
    main()
