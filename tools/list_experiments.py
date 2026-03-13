#!/usr/bin/env python3
"""
列出所有实验
用法: python tools/list_experiments.py
扫描 languages 目录下所有实验的 meta.json 文件
"""

import json
from pathlib import Path

ROOT_DIR = Path(__file__).parent.parent
LANGUAGES_DIR = ROOT_DIR / "languages"
EXPERIMENTS_DIR = ROOT_DIR / "experiments"


def scan_dir(base_dir):
    """扫描目录下所有 meta.json"""
    experiments = []

    if not base_dir.exists():
        return experiments

    for entry in base_dir.iterdir():
        if not entry.is_dir():
            continue

        meta_path = entry / "meta.json"
        if meta_path.exists():
            try:
                with open(meta_path, "r", encoding="utf-8") as f:
                    meta = json.load(f)
                # 添加路径信息
                meta["path"] = str(entry.relative_to(ROOT_DIR))
                experiments.append(meta)
            except (json.JSONDecodeError, IOError):
                pass
        else:
            # 递归扫描子目录
            experiments.extend(scan_dir(entry))

    return experiments


def format_ai_participation(value):
    """格式化 AI 参与程度"""
    mapping = {
        "full": "完全AI",
        "partial": "部分AI",
        "none": "无AI"
    }
    return mapping.get(value, value)


def format_source(value):
    """格式化来源"""
    mapping = {
        "ai": "AI生成",
        "human": "人工编写",
        "hybrid": "人机协作"
    }
    return mapping.get(value, value)


def main():
    # 扫描 languages 目录
    experiments = scan_dir(LANGUAGES_DIR)
    # 扫描 experiments 目录
    experiments.extend(scan_dir(EXPERIMENTS_DIR))

    print(f"已有实验 ({len(experiments)} 个):\n")
    print("=" * 70)

    for exp in experiments:
        print(f"\n名称: {exp.get('name', 'unknown')}")
        print(f"语言: {exp.get('language', 'unknown')}", end="")
        if exp.get("language_version"):
            print(f" ({exp['language_version']})", end="")
        print()
        print(f"描述: {exp.get('description', 'N/A')}")

        tags = exp.get("tags", [])
        if tags:
            print(f"标签: [{', '.join(tags)}]")

        print(f"创建时间: {exp.get('created', 'N/A')}")
        print(f"来源: {format_source(exp.get('source', 'N/A'))}")
        print(f"AI参与: {format_ai_participation(exp.get('ai_participation', 'N/A'))}", end="")
        if exp.get("ai_model"):
            print(f" ({exp['ai_model']})", end="")
        print()

        print(f"路径: {exp.get('path', 'N/A')}")
        print("-" * 70)


if __name__ == "__main__":
    main()
