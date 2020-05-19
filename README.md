# sudoku
C++ 实现的跨平台数独游戏。

## 特性
1. 跨平台 : Linux/Windows/macOS

## 依赖
1. CMake 3及以上

## 构建
1. Windows 下一键编译: `build.bat`
2. Linux/macOS 下一键构建: `sh build.sh` （可能需要 `chmod +x build.sh` 赋予执行权限）

## 运行
构建步骤生成的 `sudoku` 可执行文件在 `bin` 目录下
``` shell
./sudoku  # 直接启动
./sudoku -h  # 获取帮助信息
```

## 操作说明
- w 光标上移↑
- a 光标左移←
- s 光标下移↓
- d 光标右移→
- 0 删除已填入数字
- 1-9 填入数字
- enter 尝试通关
- esc 退出游戏

## Reference
This project is based on [mayerui](https://github.com/mayerui/sudoku)
