//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include "common/exception.h"
#include <limits>
#include <mutex>
#include <optional>
#include <queue>
#include <unordered_map>

namespace bustub {

// 构造函数：初始化替换器大小和参数 k，同时设置当前时间戳为 0
LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

// 淘汰页面：返回 k 距离最大或者访问时间最早的页面 ID
auto LRUKReplacer::Evict() -> std::optional<frame_id_t> {
  std::lock_guard<std::mutex> lock(latch_); // 确保线程安全

  frame_id_t candidate = -1; // 保存待淘汰的页面 ID
  size_t max_k_distance = 0; // 当前最大 k 距离
  size_t min_timestamp = std::numeric_limits<size_t>::max(); // 最早访问时间

  // 遍历所有页面的访问记录
  for (auto &[frame_id, node] : node_store_) {
    if (!node.IsEvictable()) {
      continue; // 跳过不可淘汰的页面
    }

    // 计算 k 距离：如果访问历史不足 k 次，则设置为无穷大
    size_t k_distance = (node.GetHistory().size() < k_)
                            ? std::numeric_limits<size_t>::max()
                            : current_timestamp_ - node.GetHistory().front();

    // 更新候选页面：选择 k 距离最大或者访问时间最早的页面
    if (k_distance > max_k_distance ||
       (k_distance == max_k_distance && node.GetHistory().front() < min_timestamp)) {
      max_k_distance = k_distance;
      min_timestamp = node.GetHistory().front();
      candidate = frame_id;
       }
  }
  // 如果没有合适的页面可以淘汰，返回空
  if (candidate == -1) {
    return std::nullopt;
  }

  // 淘汰页面并移除记录
  Remove(candidate);
  return candidate;
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id, AccessType access_type) {
    std::lock_guard<std::mutex> lock(latch_); // 确保线程安全

    // 检查帧 ID 的合法性
    if (frame_id >= replacer_size_) {
        throw std::out_of_range("Frame ID 超出范围。");
    }

    current_timestamp_++; // 增加全局时间戳

    // 如果页面不存在，则初始化节点
    if (node_store_.find(frame_id) == node_store_.end()) {
        node_store_.emplace(frame_id, LRUKNode(k_, frame_id));
    }

    // 获取节点并操作访问历史
    auto &node = node_store_[frame_id];
    auto &history = node.GetHistory();
    history.push_back(current_timestamp_); // 添加当前时间戳

    // 如果队列长度超过 k，移除最早的时间戳
    if (history.size() > k_) {
        history.pop_front();
    }
}


// 设置页面是否可被淘汰
void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
  std::lock_guard<std::mutex> lock(latch_); // 确保线程安全

  // 检查帧 ID 的合法性
  if (frame_id >= replacer_size_) {
    throw std::out_of_range("Frame ID 超出范围。");
  }

  // 如果页面不存在，直接返回
  if (node_store_.find(frame_id) == node_store_.end()) {
    return;
  }

  auto &node = node_store_[frame_id];
  bool was_evictable = node.IsEvictable();
  node.SetEvictable(set_evictable);

  // 根据状态变化更新可淘汰页面的计数
  if (!was_evictable && set_evictable) {
    curr_size_++;
  } else if (was_evictable && !set_evictable) {
    curr_size_--;
  }
}


void LRUKReplacer::Remove(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lock(latch_); // 确保线程安全

  // 检查帧 ID 的合法性
  if (frame_id >= replacer_size_) {
    throw std::out_of_range("Frame ID 超出范围。");
  }

  // 如果页面不存在，直接返回
  if (node_store_.find(frame_id) == node_store_.end()) {
    return;
  }

  auto &node = node_store_[frame_id];
  if (!node.IsEvictable()) {
    throw std::logic_error("试图移除不可淘汰的页面。");
  }

  // 更新可淘汰页面的计数并移除页面记录
  curr_size_--;
  node_store_.erase(frame_id);
}

// 返回当前可淘汰页面的数量
auto LRUKReplacer::Size() -> size_t {
  std::lock_guard<std::mutex> lock(latch_); // 确保线程安全
  return curr_size_;
}

}  // namespace bustub
