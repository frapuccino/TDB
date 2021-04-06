#include <glog/logging.h>

namespace tdb {

// TODO(frapuccino): support for more commands
enum Cmd {
  kGet=0,
  kSet=1,
  kInvalid=2
};

struct Meta {
  Cmd cmd;
  int timestamp;
  int sender;
  bool request;
};

// TODO(frapuccino): zero-copy need be supported
struct Message {
  Meta meta;
  StringView key;
  // TODO(frapuccino): support multi values
  StringView value;
};

}  // namespace tdb

