#include "threadpool.h"

namespace tdb {

ThreadPool::ThreadPool(int max_tasks, int num_threads)
  : max_tasks_(max_tasks), num_threads_(num_threads), running_(false) {}

ThreadPool::ThreadPool(const ThreadPoolOption& option)
  : max_tasks_(option.max_tasks), num_threads_(option.num_threads), running_(false) {}

ThreadPool::~ThreadPool() {
  if (running_) {
    Stop();
  }
}

void ThreadPool::Start() {
  if (running_) {
    return;
  }
  running_ = true;
  threads_.emplace_back(std::thread(&ThreadPool::Run, this));
}

void ThreadPool::AddTask(const Task& task) {
  std::unique_lock<std::mutex> lock(mu_);
  if(!running_) {
    return;
  }
  while (running_ && ready_tasks_.size() == max_tasks_) {
    not_full_.wait(lock);
  }
  if (!running_) {
    return;
  }
  ready_tasks_.push(task);
  not_empty_.notify_all();
}

void ThreadPool::Run() {
  while (running_) {
    Task task;
    {
      std::unique_lock<std::mutex> lock(mu_);
      while (running_ && ready_tasks_.size() == 0) {
        not_empty_.wait(lock);
      }
      if (!running_) {
        return;
      }
      task = ready_tasks_.front();
      ready_tasks_.pop();
    }
    if (task) {
      task();
    }
  }
}

void ThreadPool::Stop() {
  if (!running_) {
    return;
  }
  {
    std::unique_lock<std::mutex> lock(mu_);
    running_ = false;
    while (!ready_tasks_.empty()) {
      ready_tasks_.pop();
    }
    not_empty_.notify_all();
    not_full_.notify_all();
  }

  for (std::thread& t : threads_) {
    if (t.joinable()) {
      t.join();
    }
  }
}

}  // namespace tdb
