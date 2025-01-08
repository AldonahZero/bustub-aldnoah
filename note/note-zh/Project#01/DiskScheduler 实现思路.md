
# DiskScheduler 实现思路

## 背景概述

`DiskScheduler` 的作用是调度磁盘管理器 (`DiskManager`) 的读写请求，确保请求按照线程安全的方式被处理。核心点在于：
1. 使用共享队列存储磁盘读写请求。
2. 后台线程处理队列中的请求，调用 `DiskManager` 的 `ReadPage` 和 `WritePage` 方法。
3. 使用 `std::promise` 通知请求提交者请求处理完成。

---

## 方法功能分解

### 1. **Schedule(DiskRequest r)**
- 接收一个磁盘请求，将其加入共享队列 `request_queue_`。
- 确保线程安全。
- 确保在析构过程中不会添加请求。

### 2. **StartWorkerThread()**
- 后台线程函数，从共享队列中取出请求。
- 对于每个请求：
    - 如果是读请求，调用 `DiskManager::ReadPage()`。
    - 如果是写请求，调用 `DiskManager::WritePage()`。
    - 设置 `std::promise` 的值为 `true` 表示请求完成。
- 当队列中接收到 `std::nullopt` 时，线程退出。

---

## 数据结构设计

1. **共享队列**
    - 使用 `Channel<std::optional<DiskRequest>>`，提供线程安全的请求存储。

2. **后台线程**
    - 通过 `std::thread` 实现，负责处理请求。

---

## 实现细节

1. **Schedule**
    - 将请求加入队列。
    - 使用 `Channel` 提供的接口实现线程安全操作。

2. **StartWorkerThread**
    - 持续从队列中取出请求，直到收到 `std::nullopt`。
    - 对每个请求调用 `DiskManager` 处理，同时设置 `std::promise`。

3. **析构**
    - 确保后台线程安全终止。
    - 在析构函数中向队列插入 `std::nullopt`，通知后台线程退出。

---

## 任务总结

1. **线程安全**
    - 使用 `Channel` 确保共享数据的安全性。
    - 确保后台线程和主线程间无竞争条件。

2. **请求完成通知**
    - 使用 `std::promise` 通知提交者请求的完成状态。

3. **资源清理**
    - 在析构函数中确保后台线程退出。
    - 确保无悬空线程。

4. **异常处理**
    - 对非法请求进行验证，并抛出异常。

通过 `DiskScheduler`，实现了高效的磁盘调度。利用后台线程和共享队列，确保请求的处理顺序与线程安全。
