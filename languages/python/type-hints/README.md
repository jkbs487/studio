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
