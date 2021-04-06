#pragma once

#include <file_reader.h>
#include <stdint.h>
#include <memory>

namespace tdb {

class KvReader {
 public:
 
  KvReader(const std::string& path);

  bool NextKv(uint64_t& key_size, unsigned char*& key_start,
              uint64_t& value_size, unsigned char*& value_start);

 private:
  bool Load();
  bool SkipNBytes(size_t bytes);
  bool ReadFixed64(uint64_t* result);
  bool ReadNBytes(size_t bytes, unsigned char*& ptr);
  static constexpr size_t kMBytes = (1 << 20UL);
  static constexpr size_t kMaxBufferSize = 100 * kMBytes;
  std::shared_ptr<IoBase> io_;
  std::unique_ptr<unsigned char []> buffer_;
  unsigned char* cur_;
  unsigned char* stream_en_ptr_;
  unsigned char* buffer_st_ptr_;
  unsigned char* buffer_en_ptr_;
};

}  // namespace tdb
