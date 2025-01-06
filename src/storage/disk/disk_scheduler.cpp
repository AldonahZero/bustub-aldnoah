//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// disk_scheduler.cpp
//
// Identification: src/storage/disk/disk_scheduler.cpp
//
// Copyright (c) 2015-2023, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "storage/disk/disk_scheduler.h"
#include "common/exception.h"
#include "storage/disk/disk_manager.h"

namespace bustub {
DiskScheduler::DiskScheduler(DiskManager *disk_manager) : disk_manager_(disk_manager) {
  // TODO(P1): remove this line after you have implemented the disk scheduler API
  throw NotImplementedException(
      "DiskScheduler is not implemented yet. If you have finished implementing the disk scheduler, please remove the "
      "throw exception line in `disk_scheduler.cpp`.");

  // Spawn the background thread
  background_thread_.emplace([&] { StartWorkerThread(); });
}

DiskScheduler::~DiskScheduler() {
  // Put a `std::nullopt` in the queue to signal to exit the loop
  request_queue_.Put(std::nullopt);
  if (background_thread_.has_value()) {
    background_thread_->join();
  }
}

// Schedule 方法：调度磁盘请求
void DiskScheduler::Schedule(DiskRequest r) {
  // 将 DiskRequest 包装为 std::optional 并放入队列
  request_queue_.Put(std::make_optional<DiskRequest>(std::move(r)));
}
// StartWorkerThread 方法：后台线程处理磁盘请求
void DiskScheduler::StartWorkerThread() {
  while (true) {
    // 从队列中获取请求，阻塞等待新请求
    auto request_opt = request_queue_.Get();

    // 如果接收到 std::nullopt，退出线程
    if (!request_opt.has_value()) {
      break;
    }

    // 使用 std::move 移动构造 DiskRequest
    DiskRequest request = std::move(request_opt.value());

    // 根据请求类型调用 DiskManager 的相应方法
    if (request.is_write_) {
      disk_manager_->WritePage(request.page_id_, request.data_);
    } else {
      disk_manager_->ReadPage(request.page_id_, request.data_);
    }

    // 设置 promise 的值为 true，通知请求提交者请求已完成
    request.callback_.set_value(true);
  }
}

}// namespace bustub
