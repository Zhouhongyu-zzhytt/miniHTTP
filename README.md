# miniHTTP 🚀

这是一个基于 C++ 实现的轻量级、多线程简易 HTTP 服务器。项目从零开始构建，运行于 Linux 平台，旨在深入学习网络编程（Socket API）、并发控制（Pthreads）以及 HTTP 协议的底层原理。

## ✨ 核心特性

*   **多线程并发处理**：基于 Linux 原生 `pthread` 库实现，每当有新的客户端连接（`accept`）时，动态创建一个线程进行处理，避免主线程阻塞。
*   **HTTP 协议解析**：支持基础的 HTTP 请求解析与响应生成。
*   **静态资源服务**：支持映射本地 `wwwroot` 目录，提供 HTML、CSS 等静态文件的访问。
*   **自定义路由与业务**：通过 `custom_handle` 模块，支持针对特定路径（如登录、表单提交）编写自定义的处理逻辑。
*   **灵活的端口配置**：默认监听 80 端口，同时支持通过命令行参数动态指定运行端口。

## 📁 目录结构

```text
miniHTTP_project/
├── build/            # 编译输出目录（已被 .gitignore 忽略）
├── wwwroot/          # 静态资源根目录（存放 html, css 等）
│   ├── index.html
│   ├── login.html
│   ├── post.html
│   ├── 404.html
│   └── 样式表.css
├── CMakeLists.txt    # CMake 构建脚本
├── main.cpp          # 程序主入口，负责参数解析与启动服务
├── http.h / http.cpp # HTTP 请求解析与响应封装的核心逻辑
├── custom_handle.h   # 自定义业务处理函数的声明
├── custom_handle.cpp # 自定义业务处理函数的具体实现
└── .gitignore        # Git 忽略文件配置
