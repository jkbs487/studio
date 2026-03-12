#!/usr/bin/env python3
"""
创建新实验
用法: python tools/new_experiment.py <experiment-name> [language]
示例:
  python tools/new_experiment.py my-experiment typescript
  python tools/new_experiment.py data-analysis python
"""

import os
import sys
import json
import shutil
from pathlib import Path

TEMPLATE_DIR = Path(__file__).parent.parent / "experiments" / "_template"
EXPERIMENTS_DIR = Path(__file__).parent.parent / "experiments"


def main():
    args = sys.argv[1:]

    if not args:
        print("用法: python tools/new_experiment.py <experiment-name> [language]")
        print("")
        print("示例:")
        print("  python tools/new_experiment.py my-experiment typescript")
        print("  python tools/new_experiment.py data-analysis python")
        sys.exit(1)

    experiment_name = args[0].lower().replace(" ", "-")
    language = args[1] if len(args) > 1 else "typescript"
    target_dir = EXPERIMENTS_DIR / experiment_name

    # 检查是否已存在
    if target_dir.exists():
        print(f"错误: 实验目录已存在: {experiment_name}")
        sys.exit(1)

    # 复制模板
    print(f"创建实验: {experiment_name}")
    shutil.copytree(TEMPLATE_DIR, target_dir)

    # 更新 meta.json
    meta_path = target_dir / "meta.json"
    with open(meta_path, "r", encoding="utf-8") as f:
        meta = json.load(f)

    meta["name"] = experiment_name
    meta["created"] = __import__("datetime").date.today().isoformat()
    meta["language"] = language

    with open(meta_path, "w", encoding="utf-8") as f:
        json.dump(meta, f, ensure_ascii=False, indent=2)

    # 更新 README.md
    readme_path = target_dir / "README.md"
    with open(readme_path, "r", encoding="utf-8") as f:
        readme = f.read()

    readme = readme.replace("{实验名称}", experiment_name)

    with open(readme_path, "w", encoding="utf-8") as f:
        f.write(readme)

    print(f"✓ 实验创建成功: experiments/{experiment_name}")
    print(f"  元数据: experiments/{experiment_name}/meta.json")
    print(f"  说明: experiments/{experiment_name}/README.md")


if __name__ == "__main__":
    main()
