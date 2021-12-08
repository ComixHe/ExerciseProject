#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

constexpr size_t buffer_size = 64;
using ipv4_addr = struct sockaddr_in;
using raw_addr = struct sockaddr;

char msg_buffer[buffer_size];

int set_nonblocking(int fd) {
  auto old_opt = fcntl(fd, F_GETFL);
  auto new_opt = old_opt | O_NONBLOCK;
  assert(fcntl(fd, F_SETFL, new_opt) != -1);
  return old_opt;
}

int unblock_connect(const char *ip, const short port, int time) {
  ipv4_addr address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);
  auto sock = socket(PF_INET, SOCK_STREAM, 0);
  auto old_opt = set_nonblocking(sock);
  auto ret = connect(sock, (raw_addr *)&address, sizeof(address));
  if (ret == 0) {
    std::cout << "connect immediately" << std::endl;
    fcntl(sock, F_SETFL, old_opt);
    return sock;
  } else if (errno != EINPROGRESS) {
    std::cout << "unblock connect error" << std::endl;
    return -1;
  }

  fd_set writefds;
  struct timeval timeout;
  FD_ZERO(&writefds);
  FD_SET(sock, &writefds);

  timeout.tv_sec = time;
  timeout.tv_usec = 0;

  ret = select(sock + 1, nullptr, &writefds, nullptr, &timeout);
  if (ret <= 0) {
    std::cout << "connection time out" << std::endl;
    close(sock);
    return -1;
  }
  if (!FD_ISSET(sock, &writefds)) {
    std::cout << "no connection" << std::endl;
    close(sock);
    return -1;
  }
  auto errcode = 0;
  socklen_t length = sizeof(errcode);
  if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &errcode, &length) < 0) {
    std::cout << "getsockopt error" << std::endl;
    close(sock);
    return -1;
  }
  if (errcode != 0) {
    std::cout << "connection faild" << std::endl;
    close(sock);
    return -1;
  }
  std::cout << "connection ready" << std::endl;
  fcntl(sock, F_SETFL, old_opt);
  return sock;
}

int block_connect(const char *ip, const short port) {
  ipv4_addr server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);

  auto sock = socket(PF_INET, SOCK_STREAM, 0);
  assert(sock >= 0);
  if (connect(sock, (raw_addr *)&server_addr, sizeof(server_addr)) == -1) {
    std::cout << "connection failed" << std::endl;
    close(sock);
    return -1;
  }
  std::cout << "connection ready" << std::endl;
  return sock;
}

int main() {
  const char *ip = "192.168.122.221";
  constexpr short port = 2333;
  constexpr int out_time = 10;
#ifdef NONBLOCKING
  auto sock = unblock_connect(ip, port, out_time);
#else
  auto sock = block_connect(ip, port);
#endif
  if (sock == -1) return -1;
  auto epfd = epoll_create(5);
  epoll_event input, msg;
  input.events = EPOLLIN | EPOLLET;
  input.data.fd = STDIN_FILENO;
  msg.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
  msg.data.fd = sock;
  auto ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &msg);
  if (ret == -1) {
    std::cout << "error: " << strerror(errno) << std::endl;
    return -1;
  }
  ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &input);
  assert(ret != -1);

  int pipefd[2];
  ret = pipe(pipefd);
  assert(ret != -1);
  epoll_event events[2];
  bool running = true;

  while (running) {
    ret = epoll_wait(epfd, events, 2, -1);
    if (ret == -1) {
      std::cout << "epoll error:" << strerror(errno) << std::endl;
      break;
    }
    for (size_t i = 0; i < ret; ++i) {
      if (events[i].events & EPOLLRDHUP) {
        std::cout << events[i].data.fd << " trigger" << std::endl;
        std::cout << "peer shutdown." << std::endl;
        running = false;
        break;
      } else if (events[i].events & EPOLLIN) {
        if (events->data.fd == sock) {
          recv(sock, msg_buffer, buffer_size - 1, 0);
          std::cout << "recv msg:" << msg_buffer;
          memset(msg_buffer, 0, buffer_size);
        } else if (events->data.fd == STDIN_FILENO) {
          splice(0, nullptr, pipefd[1], nullptr, 32768,
                 SPLICE_F_MORE | SPLICE_F_MOVE);
          splice(pipefd[0], nullptr, sock, nullptr, 32768,
                 SPLICE_F_MORE | SPLICE_F_MOVE);
        }
      }
    }
  };
  close(epfd);
  close(sock);
  return 0;
}