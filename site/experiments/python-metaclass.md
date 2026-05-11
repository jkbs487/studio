---
title: python-metaclass
description: 探索 Python 元类：type、__new__、__init_subclass__、描述符、属性拦截
---

# python-metaclass

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | python |
| 版本 | 3.6+ |
| 创建时间 | 2026-03-12 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `metaclass` `meta-programming` `oop` `advanced` |

</div>

## 描述

探索 Python 元类：type、__new__、__init_subclass__、描述符、属性拦截

# Python 元类 `Python 3.6+`

探索 Python 元编程和元类的高级特性。

> **语言版本**: Python 3.6 或更高（需要 `__init_subclass__` 支持）

## 运行

```bash
python languages/python/metaclass/main.py
```

## 内容

### type 元类基础
- 使用 `type()` 动态创建类
- `type(name, bases, dict)` 语法

### 自定义元类
- 继承 `type` 创建元类
- `__new__` 方法
- 自动包装方法

### __init_subclass__ 钩子
- 简化的类定制方式
- 插件注册模式

### 描述符协议
- `__get__`, `__set__`, `__delete__`
- 属性验证描述符
- `NonEmptyString`, `PositiveNumber`

### 属性拦截
- 元类级别的 `__getattr__` / `__setattr__`

### 单例模式
- 使用元类实现单例
- `WeakValueDictionary` 管理实例

### 注册模式
- 自动注册子类
- 工厂模式实现

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
Python 元类实验
探索元编程和元类的高级用法
"""

from typing import Any, Callable
from weakref import WeakValueDictionary


# ============================================
# 1. type 元类基础
# ============================================

def demo_type_basics():
    """演示 type 作为元类的基础用法"""
    print("\n1. type 元类基础:")

    # 使用 type 动态创建类
    def greet(self):
        return f"你好, 我是 {self.name}"

    # type(name, bases, dict) 创建新类
    Person = type(
        "Person",
        (object,),
        {
            "name": "匿名",
            "greet": greet,
            "__repr__": lambda self: f"Person({self.name})"
        }
    )

    p = Person()
    p.name = "张三"
    print(f"  动态创建的类: {Person}")
    print(f"  实例: {p}")
    print(f"  方法调用: {p.greet()}")


# ============================================
# 2. 自定义元类
# ============================================

class LoggedMeta(type):
    """自动添加日志功能的元类"""

    def __new__(mcs, name: str, bases: tuple, namespace: dict):
        print(f"  [元类] 正在创建类: {name}")

        # 包装所有方法以添加日志
        for key, value in namespace.items():
            if callable(value) and not key.startswith("_"):
                namespace[key] = mcs._log_wrapper(value, key)

        return super().__new__(mcs, name, bases, namespace)

    @staticmethod
    def _log_wrapper(method: Callable, name: str) -> Callable:
        def wrapper(*args, **kwargs):
            print(f"    [LOG] 调用方法 {name}")
            result = method(*args, **kwargs)
            print(f"    [LOG] 方法 {name} 返回")
            return result
        return wrapper


class Service(metaclass=LoggedMeta):
    """使用自定义元类的类"""

    def process(self, data: str) -> str:
        return f"处理: {data}"

    def validate(self, value: int) -> bool:
        return value > 0


def demo_custom_metaclass():
    """演示自定义元类"""
    print("\n2. 自定义元类:")
    print("  创建 Service 实例...")

    service = Service()
    result = service.process("测试数据")
    print(f"  结果: {result}")


# ============================================
# 3. __init_subclass__ 钩子
# ============================================

class PluginBase:
    """使用 __init_subclass__ 的插件基类"""

    _plugins: dict[str, type] = {}

    def __init_subclass__(cls, plugin_name: str = None, **kwargs):
        super().__init_subclass__(**kwargs)

        name = plugin_name or cls.__name__
        cls._plugins[name] = cls
        print(f"  [插件注册] {name} -> {cls.__name__}")

    @classmethod
    def get_plugin(cls, name: str):
        return cls._plugins.get(name)

    @classmethod
    def list_plugins(cls):
        return list(cls._plugins.keys())


class AuthPlugin(PluginBase, plugin_name="auth"):
    """认证插件"""

    def execute(self):
        return "执行认证"


class CachePlugin(PluginBase, plugin_name="cache"):
    """缓存插件"""

    def execute(self):
        return "执行缓存"


def demo_init_subclass():
    """演示 __init_subclass__"""
    print("\n3. __init_subclass__ 钩子:")

    print(f"  已注册插件: {PluginBase.list_plugins()}")

    auth = PluginBase.get_plugin("auth")()
    print(f"  获取认证插件: {auth.execute()}")


# ============================================
# 4. 描述符协议
# ============================================

class ValidatedAttribute:
    """带验证的描述符"""

    def __init__(self, name: str, validator: Callable[[Any], bool], error_msg: str):
        self.name = name
        self.validator = validator
        self.error_msg = error_msg
        self.internal_name = f"_{name}"

    def __get__(self, obj, owner):
        if obj is None:
            return self
        return getattr(obj, self.internal_name, None)

    def __set__(self, obj, value):
        if not self.validator(value):
            raise ValueError(f"{self.name}: {self.error_msg}")
        setattr(obj, self.internal_name, value)


class NonEmptyString(ValidatedAttribute):
    """非空字符串描述符"""

    def __init__(self, name: str):
        super().__init__(
            name,
            lambda v: isinstance(v, str) and len(v.strip()) > 0,
            "必须是非空字符串"
        )


class PositiveNumber(ValidatedAttribute):
    """正数描述符"""

    def __init__(self, name: str):
        super().__init__(
            name,
            lambda v: isinstance(v, (int, float)) and v > 0,
            "必须是正数"
        )


class Product:
    """使用描述符的产品类"""

    name = NonEmptyString("name")
    price = PositiveNumber("price")

    def __init__(self, name: str, price: float):
        self.name = name
        self.price = price

    def __repr__(self):
        return f"Product(name={self.name}, price={self.price})"


def demo_descriptor():
    """演示描述符"""
    print("\n4. 描述符协议:")

    # 正常使用
    product = Product("手机", 2999)
    print(f"  创建产品: {product}")

    # 验证失败
    try:
        product.price = -100
    except ValueError as e:
        print(f"  验证失败: {e}")

    try:
        product.name = ""
    except ValueError as e:
        print(f"  验证失败: {e}")


# ============================================
# 5. 属性拦截和验证
# ============================================

class AttrAccessMeta(type):
    """拦截属性访问的元类"""

    def __getattr__(cls, name):
        print(f"  [类属性拦截] 访问不存在的属性: {name}")
        return None

    def __setattr__(cls, name, value):
        print(f"  [类属性拦截] 设置属性: {name} = {value}")
        super().__setattr__(name, value)


class Intercepted(metaclass=AttrAccessMeta):
    """使用属性拦截元类的类"""
    existing = "存在的属性"


def demo_attr_interception():
    """演示属性拦截"""
    print("\n5. 属性拦截:")

    print(f"  访问存在属性: {Intercepted.existing}")
    print(f"  访问不存在属性: {Intercepted.not_found}")


# ============================================
# 6. 单例模式
# ============================================

class SingletonMeta(type):
    """单例元类"""

    _instances: WeakValueDictionary = WeakValueDictionary()

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            print(f"  [单例] 创建新实例: {cls.__name__}")
            instance = super().__call__(*args, **kwargs)
            cls._instances[cls] = instance
        else:
            print(f"  [单例] 返回已存在实例: {cls.__name__}")
        return cls._instances[cls]


class Database(metaclass=SingletonMeta):
    """数据库连接（单例）"""

    def __init__(self):
        self.connected = False
        print("  初始化数据库连接...")

    def connect(self):
        self.connected = True
        print("  已连接数据库")


class Cache(metaclass=SingletonMeta):
    """缓存（单例）"""

    def __init__(self):
        self.data = {}


def demo_singleton():
    """演示单例模式"""
    print("\n6. 单例模式:")

    db1 = Database()
    db2 = Database()
    print(f"  db1 is db2: {db1 is db2}")

    cache1 = Cache()
    cache2 = Cache()
    print(f"  cache1 is cache2: {cache1 is cache2}")


# ============================================
# 7. 注册模式元类
# ============================================

class RegistryMeta(type):
    """自动注册类的元类"""

    registry: dict[str, type] = {}

    def __new__(mcs, name: str, bases: tuple, namespace: dict):
        cls = super().__new__(mcs, name, bases, namespace)

        # 排除基类
        if name != "RegisteredBase":
            mcs.registry[name] = cls
            print(f"  [注册] {name}")

        return cls

    @classmethod
    def get_all(mcs):
        return dict(mcs.registry)


class RegisteredBase(metaclass=RegistryMeta):
    """注册基类"""
    pass


class HandlerA(RegisteredBase):
    """处理器A"""
    def handle(self): return "A"


class HandlerB(RegisteredBase):
    """处理器B"""
    def handle(self): return "B"


def demo_registry():
    """演示注册模式"""
    print("\n7. 注册模式:")

    print(f"  已注册类: {list(RegistryMeta.registry.keys())}")

    for name, cls in RegistryMeta.registry.items():
        if name != "RegisteredBase":
            print(f"  {name}: {cls().handle()}")


# ============================================
# 主函数
# ============================================

def main():
    print("=" * 60)
    print("Python 元类实验")
    print("=" * 60)

    demo_type_basics()
    demo_custom_metaclass()
    demo_init_subclass()
    demo_descriptor()
    demo_attr_interception()
    demo_singleton()
    demo_registry()

    print("\n" + "=" * 60)
    print("实验完成!")


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
