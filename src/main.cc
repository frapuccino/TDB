#include <glog/logging.h>
#include <string>
#include "kv_reader.h"
#include "threadpool.h"
#include "allocator.h"

/*
void func1() {
  LOG(INFO) << "working in thread " << std::this_thread::get_id();
}

void func2(std::string str, int i) {
  LOG(INFO) << str << ": working in thread " << std::this_thread::get_id() << ", and id: " << i;
}
*/

int main() {
  /*
  tdb::KvReader kv_reader("/home/healy/tdb/tools/data.bin");
  size_t key_size, value_size;
  unsigned char* key_start = nullptr;
  unsigned char* value_start = nullptr;
  size_t kv_size = 0;
  while(kv_reader.NextKv(key_size, key_start, value_size, value_start)) {
    kv_size += 16 + key_size + value_size;
    // LOG(INFO) << "key_size: [" << key_size << "], value_size: [" << value_size << "]";
    // LOG(INFO) << kv_size;
  }
  LOG(INFO) << "finally: " << kv_size;
  */
  /*
  tdb::ThreadPool pool(10, 3);
  pool.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  for (int i = 0; i < 6; i++)
	{
		pool.AddTask(std::bind(func2, "fuck", i));
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
  pool.Stop();
  */
  
  Kv<>* kv = new Kv<>;
  Byte* value = const_cast<Byte*>(reinterpret_cast<const Byte*>("hello, world"));
  kv->Set(1, value);
  LOG(INFO) << kv->Get(1);

  return 0;
}
