#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <error.h>

#include <glog/logging.h>

#include "file_reader.h"

namespace tdb {

LocalIo::LocalIo(const std::string& path) {
  if ((fd_ = open(path.c_str(), O_RDONLY)) == -1) {
    LOG(FATAL) << "Open " << path << " failed, error log: " << strerror(errno);
  }

  CHECK(-1 != lseek(fd_, 0, SEEK_SET))
      << "Reset file offset failed, error log: " << strerror(errno);
}

int LocalIo::Read(void* buffer, size_t nbytes) {
  if (fd_ == -1) {
    LOG(FATAL) << "The file being read is not yet open";
  }

  int ret = read(fd_, buffer, nbytes);
  return ret;
}

void LocalIo::Reset() {
  CHECK(-1 != lseek(fd_, 0, SEEK_SET))
      << "Reset file offset failed, error log: " << strerror(errno);
}

}  // namespace tdb
