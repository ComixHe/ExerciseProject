#ifndef THREADSAEFQUEUE_HPP
#define THREADSAEFQUEUE_HPP

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

namespace ThreadPool {
template <typename T> class ThreadSafeQueue {
private:
  std::list<T> data;
  mutable std::mutex locker;
  std::condition_variable flag;
  std::atomic_bool stop_flag{false};

public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(const ThreadSafeQueue &other) {
#if __cplusplus >= 201703L
    std::scoped_lock lock(locker, other.locker);
#else
    std::lock(locker, other.locker);
    std::lock_guard<std::mutex> lock(locker, std::adopt_lock);
    std::lock_guard<std::mutex> other_lock(other.locker, std::adopt_lock);
#endif
    data = other.data;
  }
  ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;
  ThreadSafeQueue(ThreadSafeQueue &&other) = delete;
  ThreadSafeQueue &operator=(ThreadSafeQueue &&other) = delete;
  ~ThreadSafeQueue() = default;

  bool empty() const noexcept {
    std::lock_guard<std::mutex> lockholder(locker);
    return data.empty();
  }

  std::size_t size() const noexcept {
    std::lock_guard<std::mutex> lockholder(locker);
    return data.size();
  }

  template <typename F> void enqueue(F &&elem) {
    std::unique_lock<std::mutex> lockholder(locker);
    data.push_back(std::forward<F>(elem));
    lockholder.unlock();
    flag.notify_one();
  }

  bool dequeue(T &value) {
    std::unique_lock<std::mutex> lockholder(locker);
    if (data.empty())
      flag.wait(lockholder, [this] { return !data.empty(); });
    auto it = std::max_element(data.begin(), data.end());
    value = *it;
    data.erase(it);
    lockholder.unlock();
    flag.notify_one();
    return true;
  }

  void take(std::list<T> &c) {
    std::unique_lock<std::mutex> lockholder(locker);
    if (data.empty())
      std::cout << "cache is empty,async layer id:"
                << std::this_thread::get_id() << std::endl;
    flag.wait(lockholder, [this] { return !data.empty() || stop_flag; });
    c = std::move(data);
    lockholder.unlock();
    flag.notify_one();
  }

  void stop_wait() {
    std::unique_lock<std::mutex> lockholder(locker);
    stop_flag = true;
    lockholder.unlock();
    flag.notify_all();
  }

  T pop() {
    std::unique_lock<std::mutex> lockholder(locker);
    if (data.empty())
      std::cout << "cache is empty,async layer id:"
                << std::this_thread::get_id() << std::endl;
    flag.wait(lockholder, [this] { return !data.empty() || stop_flag; });
    auto result = std::max_element(data.begin(), data.end());
    data.pop_front();
    lockholder.unlock();
    return result;
  }
};
} // namespace ThreadPool
#endif