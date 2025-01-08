//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/page/b_plus_tree_page.cpp
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "storage/page/b_plus_tree_page.h"

namespace bustub {
/*
 * 检查是否是叶子页面
 * Helper methods to get/set page type
 * Page type enum class is defined in b_plus_tree_page.h
 */
auto BPlusTreePage::IsLeafPage() const -> bool {
  return page_type_ == IndexPageType::LEAF_PAGE;
}

// 设置页面类型
void BPlusTreePage::SetPageType(IndexPageType page_type) {
  page_type_ = page_type;
}

/*
 * Helper methods to get/set size (number of key/value pairs stored in that
 * page)
 */
// 获取当前页面存储的键值对数量
auto BPlusTreePage::GetSize() const -> int {
  return size_;
}

// 设置当前页面存储的键值对数量
void BPlusTreePage::SetSize(int size) {
  size_ = size;
}


// 修改页面的键值对数量（增量修改）
void BPlusTreePage::ChangeSizeBy(int amount) {
  size_ += amount;
}

/*
 * Helper methods to get/set max size (capacity) of the page
 */
// 获取页面的最大存储容量
auto BPlusTreePage::GetMaxSize() const -> int {
  return max_size_;
}

// 设置页面的最大存储容量
void BPlusTreePage::SetMaxSize(int size) {
  max_size_ = size;
}

/*
 * Helper method to get min page size
 * Generally, min page size == max page size / 2
 * But whether you will take ceil() or floor() depends on your implementation
 */
// 获取页面的最小存储容量（通常为最大容量的一半）
auto BPlusTreePage::GetMinSize() const -> int {
  return max_size_ / 2;
}
}// namespace bustub
