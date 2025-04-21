# 🚀 Multi-Threaded Proxy Server

A high-performance, multi-threaded HTTP proxy server written in C++.  
It efficiently handles concurrent client requests using a thread pool and performs request forwarding with caching support (optional).

---

## 📌 Features

- ✅ Multi-threaded request handling using thread pool
- 🌐 Acts as a transparent proxy between client and server
- ⚡ Efficient socket-based communication
- 🔄 Modular and extensible code structure
- 🧪 Benchmarking tools included for performance analysis

---

## 🧱 Project Structure
Multi-Threaded-Proxy-Server/

├── benchmark/        **# Benchmarking tools (in progress)**

├── include/          **# Header files for core classes**

├── src/              **# Source code for proxy logic and utilities**

├── main.cpp          **# Main entry point of the application**

├── makefile          **# Makefile to build the project**

└── README.md         **# Project documentation**

---

## 🧰 Tech Stack

- 🖥️ C++ (C++11 or later)
- 🧵 POSIX Threads (pthreads)
- 🔌 Berkeley Sockets API
- 🛠️ Make for build automation

---

## 🛠️ Installation

### 1. Clone the Repository

```bash
git clone https://github.com/get-akash/Multi-Threaded-Proxy-Server.git
cd Multi-Threaded-Proxy-Server
```

### 2. Build the Project
```bash
make
```

### 2. Running the Server
```bash
./proxy_server <cacheType: LRU/LFU> <cacheSize> <port>
```