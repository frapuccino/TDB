#pragma once

#include <file_reader.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <mutex>

#include "threadpool.h"

namespace tdb {

/*
template<typename Val>
class Service {
 public:
  Service();
  ~Service();
  using ReqHandle = std::function<Status(const Message& req_msg,
                                         const ReqData& req_data)>;
  void Process(const Message& message);
 private:
  ReqHandle req_handle_;
};
*/

}  // namespace tdb
