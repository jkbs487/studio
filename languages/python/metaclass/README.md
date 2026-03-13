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
