#pragma once

#include <unistd.h>
#include <string>

namespace tdb {

// TODO(frapuccino): More IO methods need to be supported
class IoBase {
    public:
    virtual int Read(void* buffer, size_t nbytes) = 0;
};

//  posix io
class LocalIo : public IoBase {

 public:

  LocalIo(const std::string& file_path);

  int Read(void* buffer, size_t nbytes) override;

  void Reset();

 private: 
  int fd_;
};

}  // namespace tdb
