#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include "ThreadSafeQueue.hpp"
#include <algorithm>
#include <any>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>
namespace ThreadPool {

class HSHAThreadpool {
  using Task = std::function<void()>;

public:
  explicit HSHAThreadpool(
      std::size_t TaskCount,
      std::size_t ThreadCount = std::thread::hardware_concurrency())
      : _TaskCount(TaskCount), _ThreadCount(ThreadCount) {
    start();
  }
  HSHAThreadpool(const HSHAThreadpool &) = delete;
  HSHAThreadpool(HSHAThreadpool &&) = delete;
  HSHAThreadpool &operator=(const HSHAThreadpool &) = delete;
  HSHAThreadpool &operator=(HSHAThreadpool &&) = delete;
  ~HSHAThreadpool() { stop(); }
  void stop() {
    std::call_once(end, [this] { this->stopthreadgroup(); });
  }
  template <typename F, typename... arg>
  void add_task(size_t pri, std::any &result, F func, arg &&...Args);
  template <typename F, typename... arg>
  void add_task(size_t pri, F func, arg &&...Args);
  template <typename F> void add_task(size_t pri, F func);

private:
  class prior_task {
  public:
    prior_task(Task &&new_func, size_t new_pri = 0)
        : priority(new_pri), func(std::move(new_func)) {}
    bool operator==(const prior_task &other) {
      return priority == other.priority;
    }
    bool operator>(const prior_task &other) {
      return priority > other.priority;
    }
    bool operator<(const prior_task &other) {
      return priority < other.priority;
    }
    void operator()() { func(); }

  private:
    size_t priority;
    Task func;
  };
  std::list<std::shared_ptr<std::thread>> threadgroup;
  ThreadSafeQueue<prior_task> Tasks;
  std::once_flag end;
  std::size_t _TaskCount;
  std::size_t _ThreadCount;
  std::atomic_bool running_flag{false};
  std::condition_variable full;
  std::condition_variable notempty;
  std::mutex threadpool_locker;
  void stopthreadgroup();
  void start();
  void runinthread();
};

void HSHAThreadpool::start() {
  running_flag = true;
  for (int i = 0; i < _ThreadCount; ++i)
    threadgroup.push_back(
        std::make_shared<std::thread>(&HSHAThreadpool::runinthread, this));
  return;
}

void HSHAThreadpool::runinthread() {
  while (running_flag) {
    std::list<prior_task> tasklist;
    Tasks.take(tasklist);
    while (!tasklist.empty()) {
      if (!running_flag)
        return;
      auto func_it = std::max_element(tasklist.begin(), tasklist.end());
      auto func = *func_it;
      func();
      tasklist.erase(func_it);
    }
  }
}

void HSHAThreadpool::stopthreadgroup() {
  running_flag = false;
  Tasks.stop_wait();
  for (auto &t : threadgroup) {
    if (t && t->joinable())
      t->join();
  }
  threadgroup.clear();
  return;
}

template <typename F, typename... arg>
void HSHAThreadpool::add_task(size_t pri, std::any &result, F func,
                              arg &&...Args) {
  std::unique_lock locker(threadpool_locker);
  if (Tasks.size() > _TaskCount)
    std::cout << "queue is full..." << std::endl;
  full.wait(locker,
            [this] { return !running_flag || Tasks.size() < _TaskCount; });
  if (!running_flag)
    return;
  Tasks.enqueue(prior_task{
      std::function<void()>{
          [&result, func, ... t_args = std::forward<arg>(Args)]() mutable {
            result = func(std::forward<arg>(t_args)...);
          }},
      pri});
  locker.unlock();
  notempty.notify_one();
}
//
template <typename F, typename... arg>
void HSHAThreadpool::add_task(size_t pri, F func, arg &&...Args) {
  std::unique_lock locker(threadpool_locker);
  if (Tasks.size() > _TaskCount)
    std::cout << "queue is full..." << std::endl;
  full.wait(locker,
            [this] { return !running_flag || Tasks.size() < _TaskCount; });
  if (!running_flag)
    return;
  Tasks.enqueue(
      prior_task{std::function<void()>{
                     [func, ... t_args = std::forward<arg>(Args)]() mutable {
                       func(std::forward<arg>(t_args)...);
                     }},
                 pri});
  locker.unlock();
  notempty.notify_one();
}

template <typename F> void HSHAThreadpool::add_task(size_t pri, F func) {
  std::unique_lock locker(threadpool_locker);
  if (Tasks.size() > _TaskCount)
    std::cout << "queue is full..." << std::endl;
  full.wait(locker,
            [this] { return !running_flag || Tasks.size() < _TaskCount; });
  if (!running_flag)
    return;
  Tasks.enqueue(prior_task{std::function<void()>{[func]() { func(); }}, pri});
  locker.unlock();
  notempty.notify_one();
}
} // namespace ThreadPool
#endif