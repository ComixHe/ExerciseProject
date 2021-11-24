#include "SimpleThreadPool.hpp"

void fun_a(std::thread::id th_id, size_t pri) {
  std::cout << "thread 2 id:" << th_id << " pri:" << pri << std::endl;
}

void test_threadpool() {
  ThreadPool::HSHAThreadpool mypool(50, 2);
  std::thread thd1([&mypool] {
    auto th_id = std::this_thread::get_id();
    for (size_t i = 0; i < 25; ++i) {
      mypool.add_task(i, [th_id, i] {
        std::cout << "thread 1 id:" << th_id << " pri:" << i << std::endl;
      });
    }
  });

  std::thread thd2([&mypool] {
    auto th_id = std::this_thread::get_id();
    for (size_t i = 25; i < 50; ++i) {
      mypool.add_task(i, fun_a, std::this_thread::get_id(), i);
    }
  });

  std::this_thread::sleep_for(std::chrono::seconds(4));
  thd1.join();
  thd2.join();
  mypool.stop();
  return;
}

int main() {
  test_threadpool();
  return 0;
}