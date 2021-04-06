#pragma once

namespace tdb {

namespace port {

// TODO(frapuccino): Consider MSC_VER and __clang__
const bool kLittleEndian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;

}  // namespace port

}  //namespace tdb
