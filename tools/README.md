# 辅助工具

此目录包含辅助 Python 脚本工具。

## 工具列表

| 脚本 | 用途 | 使用方法 |
|------|------|----------|
| `new_experiment.py` | 创建新实验 | `python tools/new_experiment.py <name>` |
| `evaluate_code.py` | 评估代码质量 | `python tools/evaluate_code.py <path>` |
| `sync_meta.py` | 同步元数据 | `python tools/sync_meta.py` |

## 依赖

工具脚本使用 Python 3 标准库，无需额外安装依赖。

## 使用示例

```bash
# 创建新实验
python tools/new_experiment.py my-experiment typescript

# 评估代码质量
python tools/evaluate_code.py experiments/my-experiment

# 同步元数据索引
python tools/sync_meta.py
```
