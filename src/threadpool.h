#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <vector>

namespace tdb {

using Task = std::function<void()>;

struct ThreadPoolOption {
  std::string name;  // name of thread pool
  int num_threads;
  int max_tasks;
};

class ThreadPool {
 public:
  explicit ThreadPool() = delete;
  explicit ThreadPool(int max_tasks, int num_threads);
  explicit ThreadPool(const ThreadPoolOption& option);
  ~ThreadPool();
  void Start();
  void Stop();
  void AddTask(const Task& task);
  void Run();

 private:
  bool running_;
  int max_tasks_;
  int num_threads_;
  std::mutex mu_;
  std::condition_variable not_empty_;
  std::condition_variable not_full_;
  std::queue<Task> ready_tasks_;
  std::vector<std::thread> threads_;
};

}  // namespace tdb
