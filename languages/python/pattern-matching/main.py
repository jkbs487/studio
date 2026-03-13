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
