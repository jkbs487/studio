"""
算法验证入口脚本。

用法（在 experiments/algorithm-framework/ 目录下执行）:
    python run.py                 # 验证 solutions/ 下所有算法
    python run.py two-sum         # 只验证指定算法
    python run.py --bench         # 验证全部并附加基准测试
    python run.py two-sum --bench # 验证指定算法并附加基准测试

新增算法流程:
    1. 在 solutions/ 下新建 .py 文件（可复制 solutions/example.py 模板）
    2. 用 @solve 注册算法，用 register_cases 绑定用例
    3. 运行本脚本即可
"""

import argparse
import importlib.util
import sys
from pathlib import Path

from algoframe import REGISTRY, bench_solution, run_solution

SOLUTIONS_DIR = Path(__file__).parent / "solutions"


def load_solutions() -> None:
    """导入 solutions/ 下所有 .py 文件，触发其中的 @solve 注册。"""
    if not SOLUTIONS_DIR.exists():
        return
    for path in sorted(SOLUTIONS_DIR.glob("*.py")):
        if path.name.startswith("_"):
            continue
        spec = importlib.util.spec_from_file_location(path.stem, path)
        if spec is None or spec.loader is None:
            continue
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)


def main() -> int:
    parser = argparse.ArgumentParser(description="算法验证入口")
    parser.add_argument("names", nargs="*", help="要运行的算法名称（默认全部）")
    parser.add_argument("--bench", action="store_true", help="附加基准测试")
    args = parser.parse_args()

    load_solutions()

    if not REGISTRY:
        print("没有找到任何算法。请在 solutions/ 目录下创建 .py 文件。")
        return 1

    selected = args.names or list(REGISTRY)
    missing = [n for n in args.names if n not in REGISTRY]
    if missing:
        print(f"未找到算法: {', '.join(missing)}")
        print(f"可用算法: {', '.join(REGISTRY)}")
        return 1

    total_pass = total_cases = 0
    failed: list[str] = []

    for name in selected:
        sol = REGISTRY[name]
        header = f"== {name}"
        if sol.desc:
            header += f" - {sol.desc}"
        print(header)

        if not sol.cases:
            print("  (无测试用例，跳过验证)")
        else:
            passed, total, elapsed = run_solution(sol)
            total_pass += passed
            total_cases += total
            print(f"  结果: {passed}/{total} 通过, 耗时 {elapsed * 1000:.1f} ms")
            if passed < total:
                failed.append(name)

        if args.bench:
            if sol.bench_cases:
                bench_solution(sol)
            else:
                print("  (无基准用例，跳过基准测试)")

        print()

    if total_cases:
        print(f"汇总: {total_pass}/{total_cases} 通过")
        if failed:
            print(f"失败算法: {', '.join(failed)}")
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
