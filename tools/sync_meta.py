#!/usr/bin/env python3
"""
同步元数据
扫描所有实验目录，更新元数据索引
用法: python tools/sync_meta.py
"""

import json
from pathlib import Path
from datetime import datetime


EXPERIMENTS_DIR = Path(__file__).parent.parent / "experiments"
INDEX_FILE = EXPERIMENTS_DIR / "index.json"


def main():
    print("同步实验元数据...\n")

    experiments = []

    for entry in EXPERIMENTS_DIR.iterdir():
        if not entry.is_dir():
            continue
        if entry.name.startswith("_") or entry.name.startswith("."):
            continue

        meta_path = entry / "meta.json"

        if meta_path.exists():
            try:
                with open(meta_path, "r", encoding="utf-8") as f:
                    meta = json.load(f)

                meta["path"] = entry.name
                experiments.append(meta)
                print(f"✓ {entry.name}")
            except (json.JSONDecodeError, IOError) as e:
                print(f"✗ {entry.name} - 元数据解析失败: {e}")
        else:
            print(f"⚠ {entry.name} - 缺少 meta.json")

    # 按创建日期排序
    experiments.sort(key=lambda x: x.get("created", ""), reverse=True)

    # 写入索引
    index_data = {
        "updated": datetime.now().isoformat(),
        "count": len(experiments),
        "experiments": experiments
    }

    with open(INDEX_FILE, "w", encoding="utf-8") as f:
        json.dump(index_data, f, ensure_ascii=False, indent=2)

    print(f"\n完成! 共 {len(experiments)} 个实验")
    print(f"索引文件: {INDEX_FILE}")


if __name__ == "__main__":
    main()
