# Fast Exchange - 快速汇率换算

一个基于 Qt6 的轻量级汇率换算工具，支持 USD、EUR、AED、CNY、HKD、TWD 六种货币之间的实时转换。

## 功能

- 支持 USD / EUR / AED / CNY / HKD / TWD 六种货币互转
- 从 [open.er-api.com](https://open.er-api.com) 获取实时汇率，离线时使用默认汇率
- 显示汇率更新时间
- 中文 / English 界面切换
- 跨平台适配（Windows / macOS / Linux）

## 环境要求

- Qt 6
- CMake 3.16+
- C++17 编译器

## 编译与运行

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/platform
cmake --build .
./FastExchange
```

## 项目结构

```
Fast-Exchange/
├── CMakeLists.txt        # 构建配置
├── main.cpp              # 程序入口，平台字体设置
├── exchangewidget.h      # 主界面头文件
├── exchangewidget.cpp    # 主界面实现
└── README.md
```

## 汇率数据源

使用 [Exchange Rate API](https://open.er-api.com) 免费接口，无需 API Key。
