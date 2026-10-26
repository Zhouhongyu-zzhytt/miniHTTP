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
```

## 🛠️ 环境要求

*  **操作系统**：Linux（推荐 CentOS 7 或 Ubuntu）
*  **编译器**：GCC（支持 C++11 及以上，CentOS 7 默认 GCC 4.8.5 即可编译，建议使用 devtoolset 升级获得更好体验）
*  **构建工具**：CMake (3.0+)
*  **依赖库**：pthread (Linux 原生线程库)

## 🚀 快速开始

* **1. 克隆项目**
```bash
git clone https://github.com/Zhouhongyu-zzhytt/miniHTTP.git
cd miniHTTP
```
* **2. 编译项目**
```bash
mkdir build && cd build
cmake ..
make
```
* **3. 运行服务器**
```bash
# 默认监听 80 端口运行（需要 root 权限）
sudo ./miniHTTP

# 或者指定端口号运行（例如 8080）
./miniHTTP 8080
```
* **4. 访问测试**
打开浏览器，访问 http://127.0.0.1 （或服务器对应的 IP 地址及端口），即可看到 wwwroot 下的默认页面。

## 🧠 核心技术点解析
*  **多线程并发模型**：主线程通过 accept() 阻塞等待客户端连接；一旦建立连接，调用 pthread_create() 创建分离线程（Detached Thread）处理该连接的读写，从而支持多客户端同时访问。
*  **Socket网络通信**：使用 Linux 原生 Socket API (socket, bind, listen, accept, recv, send) 建立 TCP 连接并进行数据交互。
*  **HTTP报文处理**：解析请求行、请求头，根据请求路径映射本地 wwwroot 文件或调用 custom_handle 生成响应报文。

## 📄 开源许可
项目基于 MIT License 协议开源。欢迎 Star、Fork 和提交 PR！



  
  



