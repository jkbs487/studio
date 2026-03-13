#!/bin/bash

# 经典性能对比实验 - 统一构建脚本
# 用法: ./run.sh [build|run|clean|all]

set -e
cd "$(dirname "$0")"

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 生成的文件
CPP_BIN="benchmark_cpp"
GO_BIN="benchmark_go"
TEMP_FILES="$CPP_BIN $GO_BIN"

build() {
    echo -e "${YELLOW}>>> 编译所有版本...${NC}"
    
    echo "编译 C++ 版本..."
    g++ -std=c++11 -O2 -o $CPP_BIN main.cpp
    
    echo "编译 Go 版本..."
    go build -ldflags="-s -w" -o $GO_BIN main.go
    
    echo -e "${GREEN}>>> 编译完成${NC}"
}

run() {
    echo ""
    echo "========================================"
    echo "  经典性能对比实验 - Go vs C++ vs Python"
    echo "========================================"
    
    echo ""
    echo -e "${YELLOW}>>> 运行 C++ 版本...${NC}"
    echo "----------------------------------------"
    ./$CPP_BIN
    
    echo ""
    echo -e "${YELLOW}>>> 运行 Go 版本...${NC}"
    echo "----------------------------------------"
    ./$GO_BIN
    
    echo ""
    echo -e "${YELLOW}>>> 运行 Python 版本...${NC}"
    echo "----------------------------------------"
    python3 main.py
    
    echo ""
    echo "========================================"
    echo "  性能对比完成"
    echo "========================================"
}

clean() {
    echo -e "${YELLOW}>>> 清理生成文件...${NC}"
    rm -f $TEMP_FILES
    echo -e "${GREEN}>>> 清理完成${NC}"
}

case "${1:-all}" in
    build)
        build
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    all|"")
        build
        run
        ;;
    *)
        echo "用法: $0 [build|run|clean|all]"
        echo "  build - 编译所有版本"
        echo "  run   - 运行所有测试"
        echo "  clean - 清理生成文件"
        echo "  all   - 编译并运行（默认）"
        exit 1
        ;;
esac
