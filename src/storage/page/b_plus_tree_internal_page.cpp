//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/page/b_plus_tree_internal_page.cpp
//
// Copyright (c) 2018-2024, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <sstream>

#include "common/exception.h"
#include "storage/page/b_plus_tree_internal_page.h"

namespace bustub {
/*****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/
/*
 * Init method after creating a new internal page
 * Including set page type, set current size, and set max page siz
 * 初始化内部页面。
 * @param max_size 最大容量e
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Init(int max_size) {
  SetPageType(IndexPageType::INTERNAL_PAGE);  // 设置页面类型为内部页面
  SetSize(0);                                // 初始化页面大小为 0
  SetMaxSize(max_size);                      // 设置页面最大容量
}

/*
 * Helper method to get/set the key associated with input "index" (a.k.a
 * array offset)
 * 获取指定索引的键。
 * @param index 键的索引，必须大于 0
 * @return 指定索引的键
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::KeyAt(int index) const -> KeyType {
  BUSTUB_ASSERT(index > 0 && index < GetSize(), "Invalid index for KeyAt.");
  return key_array_[index];
}


/**
 * 设置指定索引的键。
 * @param index 键的索引，必须大于 0
 * @param key 要设置的新键值
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::SetKeyAt(int index, const KeyType &key) {
  BUSTUB_ASSERT(index > 0 && index < GetSize(), "Invalid index for SetKeyAt.");
  key_array_[index] = key;
}
/**
 * 获取指定值的索引。
 * @param value 要查找的值
 * @return 指定值的索引
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueIndex(const ValueType &value) const -> int {
  for (int i = 0; i < GetSize(); i++) {
    if (page_id_array_[i] == value) {
      return i;
    }
  }
  return -1;  // 如果未找到，返回 -1
}

/**
 * 获取指定索引的值。
 * @param index 值的索引
 * @return 指定索引的值
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueAt(int index) const -> ValueType {
  BUSTUB_ASSERT(index >= 0 && index < GetSize(), "Invalid index for ValueAt.");
  return page_id_array_[index];
}

// valuetype for internalNode should be page id_t
template class BPlusTreeInternalPage<GenericKey<4>, page_id_t, GenericComparator<4>>;
template class BPlusTreeInternalPage<GenericKey<8>, page_id_t, GenericComparator<8>>;
template class BPlusTreeInternalPage<GenericKey<16>, page_id_t, GenericComparator<16>>;
template class BPlusTreeInternalPage<GenericKey<32>, page_id_t, GenericComparator<32>>;
template class BPlusTreeInternalPage<GenericKey<64>, page_id_t, GenericComparator<64>>;
}  // namespace bustub
