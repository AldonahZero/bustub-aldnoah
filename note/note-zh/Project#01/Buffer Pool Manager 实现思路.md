# Buffer Pool Manager 实现思路

## 背景概述

Buffer Pool Manager 是数据库系统的重要组件，用于管理从磁盘加载到内存中的数据页。它通过使用 LRU-K 替换算法（LRUKReplacer）和磁盘调度器（DiskScheduler），在内存和磁盘之间高效调度页面数据。核心功能包括：
1. 管理页面的加载、访问和写回。
2. 确保线程安全，避免多线程环境中的数据竞态问题。
3. 提供页面访问的 RAII 接口（ReadPageGuard 和 WritePageGuard）。

---

## 功能模块

### 1. LRU-K 替换策略
- 使用 LRUKReplacer 管理页面替换。
- 根据页面的访问历史决定淘汰策略。

### 2. 磁盘调度
- 通过 DiskScheduler 调用磁盘操作（读写页面）。
- 调用 `DiskManager::ReadPage` 和 `DiskManager::WritePage` 进行物理操作。

### 3. 线程安全
- 通过互斥锁（`std::mutex`）保护共享资源。
- 确保同一时间只有一个线程操作页面或替换算法。

---

## 方法功能分解

### **BufferPoolManager 方法**

1. **NewPage()**
    - 创建一个新页面。
    - 如果缓冲池已满，使用 LRU-K 替换策略淘汰页面。

2. **DeletePage(page_id_t page_id)**
    - 删除一个页面。
    - 确保没有线程正在访问被删除的页面。

3. **CheckedReadPage(page_id_t page_id)**
    - 获取一个页面的读访问权限。
    - 返回 `ReadPageGuard`，确保访问期间页面不会被替换。

4. **CheckedWritePage(page_id_t page_id)**
    - 获取一个页面的写访问权限。
    - 返回 `WritePageGuard`，确保访问期间页面不会被替换。

5. **FlushPage(page_id_t page_id)**
    - 将页面从内存刷写到磁盘。

6. **FlushAllPages()**
    - 刷写所有内存中的页面到磁盘。

7. **GetPinCount(page_id_t page_id)**
    - 获取页面的当前引用计数。

---

### **Page Guard 类**

1. **ReadPageGuard**
    - 确保页面的线程安全读访问。
    - RAII 模式，自动管理页面的 pin count。

2. **WritePageGuard**
    - 确保页面的线程安全写访问。
    - RAII 模式，自动管理页面的 pin count 和脏标记。

---

## 数据结构设计

1. **Page Table**
    - 使用 `std::unordered_map<page_id_t, FrameHeader *>` 映射页面 ID 到内存帧。

2. **FrameHeader**
    - 包含页面的元信息（如脏标记、pin count）。

3. **LRUKReplacer**
    - 管理页面的替换策略。

4. **DiskScheduler**
    - 调度磁盘读写操作。

---

## 线程安全

1. **锁保护**
    - 使用 `std::mutex` 保护共享数据。
    - 在方法调用开始时加锁，结束时解锁。

2. **引用计数**
    - 使用 `FrameHeader::pin_count_` 确保页面在被访问时不会被淘汰。

---

## 实现策略

1. **替换页面**
    - 如果缓冲池已满，使用 LRUKReplacer 淘汰页面。
    - 如果被淘汰页面为脏页，先写回磁盘。

2. **刷写页面**
    - 在页面被淘汰或显式调用 `FlushPage` 时，将脏页面写回磁盘。

3. **RAII**
    - 使用 `ReadPageGuard` 和 `WritePageGuard` 自动管理页面的 pin count 和锁。
