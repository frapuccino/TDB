#pragma once
#include <string>

namespace tdb {

class StringView {
 public:
  StringView() = delete;
  StringView(unsigned char* ptr, size_t length)
    : ptr_(ptr), length_(length) {}
  StringView(const StringView& sp) {
    this->ptr_ = sp.ptr_;
    this->length_ = sp.length_;
  }
  
  bool empty() { return length_ == 0; }

  unsigned char* ptr() { return ptr_; }

  size_t length() { return length_; }

  bool RemovePrefix(size_t bytes) {
    if (length_ < bytes) {
      return false;
    }
    ptr_ += bytes;
    length_ -= bytes;
    return true;
  }

  void ConvertToString(std::string* result) {
    *result = std::move(std::string(ptr_, ptr_ + length_));
  }

 private:
  unsigned char* ptr_;
  size_t length_;
}
}
