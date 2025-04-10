# 连连看小游戏 (C++ & Qt6)

[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)
![Framework](https://img.shields.io/badge/Framework-Qt6-brightgreen.svg)

## 简介

这是一个使用 C++ 语言和 Qt6 框架开发的简单连连看小游戏.

## 使用的技术

* **C++:** 主要编程语言. 
* **Qt6:** GUI 框架. 
* **Inno Setup:** 用于创建 Windows 安装程序. 

## 如何构建并运行

### 前提条件

* 安装了 Qt 6 开发环境. 
* 安装了 C++ 编译器 (例如 GCC,  Clang). 

### 步骤

1.  **克隆代码仓库：**
    ```bash
    git clone https://github.com/mikami-w/QtLianliankan.git
    cd QtLianliankan
    ```

2.  **使用 Qt Creator 构建项目 (推荐):**
    * 使用 Qt Creator 打开 `CMakeLists.txt` 文件. 
    * 配置构建套件 (Kit). 
    * 点击 "构建" (Build) -> "构建所有" (Build All). 
    * 点击 "运行" (Run). 

3.  **或者, 使用命令行构建：**
    * 打开终端或命令提示符, 导航到项目根目录. 
    * 运行 `qmake -project` (如果还没有 `.pro` 文件). 
    * 运行 `qmake`. 
    * **在 Linux/macOS:** 运行 `make`. 
    * **在 Windows (MinGW):** 运行 `mingw32-make`. 
    * 构建成功后, 在 `debug` 或 `release` 目录下找到可执行文件并运行. 

## 开源许可

本项目使用 **MIT License**.  详细内容请查看 [LICENSE](LICENSE) 文件. 
