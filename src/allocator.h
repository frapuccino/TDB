#include <memory>
#include <string>
#include <cstring>
#include <unordered_map>

#define MEM_SIZE (1 << 20)

typedef unsigned char Byte;
template <typename T, typename ALLOC = std::allocator<T>>
class Memory {
 public:
  using traits_ = typename std::allocator_traits<ALLOC>::template rebind_traits<T>;
  using allocator_type = typename traits_::allocator_type;

  Memory() = delete;

  Memory(std::size_t cap, const allocator_type& alloc = ALLOC())
   : cap_(cap), allocator_(alloc) {
    data_ = allocator_.allocate(cap);
    cursor_ = 0;
    ptr_ = data_;
  }

  ~Memory(void) {
    if (data_) {
      allocator_.deallocate(data_, cap_);
      cap_ = cursor_ = 0;
      data_ = ptr_ = nullptr;
    }
  }

  T* Malloc(std::size_t bytes) {
    if (cursor_ + bytes > cap_) {
      cursor_ = cap_;
      return nullptr;
    }
    T* __p = ptr_;
    ptr_ += bytes;
    cursor_ += bytes;
    return __p;
  }

  void Free(T* p) {
    LOG(FATAL) << "Free() is not yet supported";
  }

 private:
  T* data_;
  allocator_type allocator_;
  T* ptr_;
  std::size_t cursor_;
  std::size_t cap_;
};

/*
template <typename V, typename ALLOC = std::allocator<V>>
class Kv {
  static_assert(false, "");
};
*/

// TODO(frapuccino): more value type should be supported
template <typename ALLOC = std::allocator<Byte>>
class Kv {
 private:
  using traits_ = typename std::allocator_traits<ALLOC>::template rebind_traits<Byte>;
  using allocator_type = typename traits_::allocator_type;
  using mem_spec_t = Memory<Byte, ALLOC>;
  using mem_allocator_t = typename traits_::template rebind_alloc<mem_spec_t>;
  using mem_traits_ = typename traits_::template rebind_traits<mem_spec_t>;
  using mem_pointer = typename mem_traits_::pointer;
 public:
  Kv(const std::size_t& cap = MEM_SIZE, const allocator_type& alloc = ALLOC())
   : allocator_(alloc) {
    {
      mem_allocator_t __alloc(allocator_);
      auto* __p = __alloc.allocate(1);
      __alloc.construct(__p, cap);
      memory_.reset(__p, [__alloc](mem_pointer p) mutable {
        __alloc.destroy(p);
        __alloc.deallocate(p, 1);
      });
    }
  }

  ~Kv() { };
  Byte* Get(uint64_t key) {
    auto it = kv_.find(key);
    if (it == kv_.end()) {
      return nullptr;
    }
    return it->second;
  }

  // 
  void Set(uint64_t key, Byte* value) {
    int len = strlen(value);
    Byte* val = memory_->Malloc(len + 1);
    std::memmove(val, value, len);
    *(val + len) = 0;
    kv_.insert(std::make_pair(key, val));
  }
 private:
  std::unordered_map<uint64_t, Byte*> kv_;
  allocator_type allocator_;
  std::shared_ptr<Memory<Byte>> memory_;
};
