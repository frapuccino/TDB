#include <glog/logging.h>
#include <cstring>

#include "kv_reader.h"
#include "byte_order.h"

namespace tdb {

KvReader::KvReader(const std::string& path)
 : buffer_(new unsigned char[kMaxBufferSize + 1]) {
  io_ = std::make_shared<LocalIo>(path);

  cur_ = buffer_st_ptr_ = stream_en_ptr_  = 
      (unsigned char*)(&buffer_[0]);
  buffer_en_ptr_ = (unsigned char*)(&buffer_[kMaxBufferSize]);
}

bool KvReader::Load() {
  CHECK(cur_ <= stream_en_ptr_);
  size_t remainder = stream_en_ptr_ - cur_;
  std::memcpy(buffer_st_ptr_, cur_, remainder);
  cur_ = buffer_st_ptr_;
  stream_en_ptr_ = buffer_st_ptr_ + remainder;

  int read_bytes = io_->Read(stream_en_ptr_, buffer_en_ptr_ - stream_en_ptr_);

  if (read_bytes == 0) {
    return false;
  }
  stream_en_ptr_ = stream_en_ptr_ + read_bytes;
  return true;
}

bool KvReader::SkipNBytes(size_t bytes) {
  if (cur_ + bytes > stream_en_ptr_) {
    if(!Load()) {
      return false;
    }
  }
    
  if (cur_ + bytes > stream_en_ptr_) {
    return false;
  }

  cur_ = cur_ + bytes;
  return true;
}

bool KvReader::ReadFixed64(uint64_t* result) {
  CHECK(result != nullptr);
  if (cur_ + 8 > stream_en_ptr_) {
    if(!Load()) {
      return false;
    }
  }
  if (cur_ + 8 > stream_en_ptr_) {
    return false;
  }

  if (port::kLittleEndian) {
    memcpy(result, cur_, sizeof(uint64_t));
  } else {
    *result = ((static_cast<uint64_t>(cur_[0])) |
               (static_cast<uint64_t>(cur_[1]) << 8) |
               (static_cast<uint64_t>(cur_[2]) << 16) |
               (static_cast<uint64_t>(cur_[3]) << 24) |
               (static_cast<uint64_t>(cur_[4]) << 32) |
               (static_cast<uint64_t>(cur_[5]) << 40) |
               (static_cast<uint64_t>(cur_[6]) << 48) |
               (static_cast<uint64_t>(cur_[7]) << 56));
  }
  cur_ += 8;
  return true;
}

bool KvReader::ReadNBytes(size_t bytes, unsigned char*& ptr) {
  if (cur_ + bytes > stream_en_ptr_) {
    if(!Load()) {
      return false;
    }
  }
  if (cur_ + bytes > stream_en_ptr_) {
    return false;
  }
  
  ptr = cur_;
  cur_ += bytes;
  return true;
}


bool KvReader::NextKv(uint64_t& key_size, unsigned char*& key_start,
                     uint64_t& value_size, unsigned char*& value_start) {

  // read key size
  bool more = ReadFixed64(&key_size);
  if (!more) {
    if (cur_ == stream_en_ptr_) {
      return false;
    } else {
      LOG(FATAL) << "failed to parse key_size: [incomplete byte stream]";
    }
  }

  // read key
  more = ReadNBytes(key_size, key_start);
  if (!more) {
    LOG(FATAL) << "key has no more bytes to read, expected key_size: ["
               << key_size << "], actual: [" << stream_en_ptr_ - cur_ << "]";
  }

  // read value size
  more = ReadFixed64(&value_size);
  if (!more) {
    LOG(FATAL) << "failed to parse value_size: [incomplete byte stream]";
  }

  // read value
  more = ReadNBytes(value_size, value_start);
  if (!more) {
    LOG(FATAL) << "value has no more bytes to read, expected value_size: ["
               << value_size << "], actual: [" << stream_en_ptr_ - cur_ << "]";
  }
  return true;
}

}  // namespace tdb
