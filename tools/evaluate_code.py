#!/usr/bin/env python3
"""
评估代码质量
用法: python tools/evaluate_code.py <path>
示例:
  python tools/evaluate_code.py experiments/my-experiment
"""

import os
import sys
import re
from pathlib import Path
from dataclasses import dataclass
from typing import List


@dataclass
class EvaluationResult:
    file: str
    lines: int
    functions: int
    classes: int
    comments: int
    complexity: str
    issues: List[str]


CODE_EXTENSIONS = {".ts", ".tsx", ".js", ".jsx", ".py", ".rs", ".go", ".java"}


def main():
    args = sys.argv[1:]

    if not args:
        print("用法: python tools/evaluate_code.py <path>")
        print("")
        print("示例:")
        print("  python tools/evaluate_code.py experiments/my-experiment")
        sys.exit(1)

    target_path = Path(args[0]).resolve()

    if not target_path.exists():
        print(f"错误: 路径不存在: {target_path}")
        sys.exit(1)

    print(f"评估代码: {target_path}\n")

    results: List[EvaluationResult] = []

    if target_path.is_dir():
        evaluate_directory(target_path, results)
    else:
        evaluate_file(target_path, results)

    # 打印结果
    print("评估结果:")
    print("─" * 60)

    for result in results:
        print(f"\n📁 {result.file}")
        print(f"   行数: {result.lines}")
        print(f"   函数: {result.functions}")
        print(f"   类: {result.classes}")
        print(f"   注释: {result.comments}")
        print(f"   复杂度: {result.complexity}")

        if result.issues:
            print(f"   ⚠️ 问题:")
            for issue in result.issues:
                print(f"      - {issue}")

    # 总结
    total_lines = sum(r.lines for r in results)
    total_comments = sum(r.comments for r in results)
    comment_rate = (total_comments / total_lines * 100) if total_lines > 0 else 0

    print("\n" + "─" * 60)
    print("总结:")
    print(f"  文件数: {len(results)}")
    print(f"  总行数: {total_lines}")
    print(f"  平均注释率: {comment_rate:.1f}%")


def evaluate_directory(dir_path: Path, results: List[EvaluationResult]):
    for item in dir_path.rglob("*"):
        if item.is_dir():
            if item.name in ("node_modules", ".git") or item.name.startswith("."):
                continue
        elif item.is_file() and item.suffix in CODE_EXTENSIONS:
            evaluate_file(item, results)


def evaluate_file(file_path: Path, results: List[EvaluationResult]):
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    lines = content.split("\n")

    # 简单分析
    function_matches = len(re.findall(
        r"function\s+\w+|def\s+\w+|fn\s+\w+|const\s+\w+\s*=\s*(?:async\s*)?\(|=>\s*{",
        content
    ))
    class_matches = len(re.findall(r"class\s+\w+", content))
    comment_matches = len(re.findall(r"//.*|#.*|'''[\s\S]*?'''|\"\"\"[\s\S]*?\"\"\"", content))

    # 检测问题
    issues = []

    if len(lines) > 500:
        issues.append("文件过长，建议拆分")

    if comment_matches < len(lines) * 0.1:
        issues.append("注释较少，建议添加更多说明")

    todo_matches = re.findall(r"TODO|FIXME|XXX", content, re.IGNORECASE)
    if todo_matches:
        issues.append(f"存在 {len(todo_matches)} 个待办项 (TODO/FIXME)")

    # 计算复杂度
    complexity = "high" if function_matches > 20 else "medium" if function_matches > 10 else "low"

    results.append(EvaluationResult(
        file=str(file_path.relative_to(Path.cwd())),
        lines=len(lines),
        functions=function_matches,
        classes=class_matches,
        comments=comment_matches,
        complexity=complexity,
        issues=issues
    ))


if __name__ == "__main__":
    main()
