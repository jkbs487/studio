---
title: python-pattern-matching
description: 探索 Python 3.10+ 模式匹配语法：match/case
---

# python-pattern-matching

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | python |
| 版本 | N/A |
| 创建时间 | 2026-03-13 |
| 来源 | ai |
| AI 参与 | full |
| 标签 | `pattern-matching` `python310` `advanced` |

</div>

## 描述

探索 Python 3.10+ 模式匹配语法：match/case

# Python 模式匹配 (Pattern Matching) `Python 3.10+`

探索 Python 3.10 引入的 `match/case` 语法。

> **语言版本**: Python 3.10 或更高

## 运行

```bash
python languages/python/pattern-matching/main.py
```

## 内容

### 基础 match
- `match value:` 语法
- `case pattern:` 分支
- `_` 通配符

### 组合模式
- `|` 或运算符
- `case 200 | 201:`

### 序列模式
- 列表/元组解构
- `*rest` 剩余元素

### 映射模式
- 字典解构
- 部分匹配

### 类模式
- `@dataclass` 解构
- 位置参数匹配

### 守卫
- `if` 条件守卫
- `case pattern if condition:`

### 类型匹配
- `int()`, `str()` 类型检查
- `as` 绑定

## 目录

- `meta.json` - 实验元数据
- `README.md` - 本文档
- `main.py` - 实验代码


## 源码

<details>
<summary>main.py</summary>

```python
#!/usr/bin/env python3
"""Python 模式匹配实验 (需要 Python 3.10+)"""

import sys
if sys.version_info < (3, 10):
    print("=" * 50)
    print("需要 Python 3.10+")
    print(f"当前: Python {sys.version_info.major}.{sys.version_info.minor}")
    print("请运行: pyenv local 3.10")
    print("=" * 50)
    sys.exit(1)

from dataclasses import dataclass

def main():
    print("=" * 50)
    print("Python 模式匹配实验 (3.10+)")
    print("=" * 50)

    # 1. 基础 match
    def http_status(s: int) -> str:
        match s:
            case 200: return "OK"
            case 404: return "Not Found"
            case 500: return "Error"
            case _: return "Unknown"
    print("\n1. 基础:", http_status(200), http_status(404))

    # 2. 组合模式
    def group(s: int) -> str:
        match s:
            case 200 | 201: return "Success"
            case 400 | 404: return "Client Error"
            case _: return "Other"
    print("2. 组合:", group(200), group(404))

    # 3. 序列模式
    def cmd(c: list) -> str:
        match c:
            case ["quit"]: return "退出"
            case ["help", t]: return f"帮助: {t}"
            case ["load", f, *opts]: return f"加载 {f}"
            case _: return "未知"
    print("3. 序列:", cmd(["help", "api"]), cmd(["load", "a.txt", "-f"]))

    # 4. 映射模式
    def cfg(d: dict) -> str:
        match d:
            case {"mode": "prod"}: return "生产"
            case {"mode": "dev", "debug": True}: return "开发+调试"
            case _: return "其他"
    print("4. 映射:", cfg({"mode": "prod"}), cfg({"mode": "dev", "debug": True}))

    # 5. 类模式
    @dataclass
    class Point:
        x: float
        y: float

    def shape(s) -> str:
        match s:
            case Point(0, 0): return "原点"
            case Point(x, y) if x == y: return f"对角点({x},{y})"
            case Point(x, y): return f"点({x},{y})"
            case _: return "?"
    print("5. 类:", shape(Point(0, 0)), shape(Point(3, 3)))

    # 6. 守卫
    @dataclass
    class User:
        name: str
        age: int

    def perm(u: User, act: str) -> str:
        match u, act:
            case User(role="admin"), "del": return "可删除"
            case User(age=a), "del" if a < 18: return "禁止"
            case _: return "普通"
    print("6. 守卫:", perm(User("admin", 30, "admin"), "del"))

    # 7. 类型匹配
    def typ(v) -> str:
        match v:
            case int() as n if n > 0: return f"正整数{n}"
            case str() as s: return f"字符串({len(s)}字符)"
            case _: return "其他"
    print("7. 类型:", typ(42), typ("hi"))

    print("\n" + "=" * 50)
    print("完成!")

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
