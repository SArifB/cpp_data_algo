#pragma once
#include <utility>

#include "cont.hpp"
#include "box_ptr.hpp"

namespace my {

template <typename T>
struct ForwardList {
 private:
  struct Node {
    T data{};
    Box<Node> next{};
  };

  Box<Node> head{};

 public:
  ForwardList() = default;

  ~ForwardList() noexcept { clean(); }

  auto append(T &&_data) {
    auto tmp = Box{new Node{std::move(_data)}};
    tmp->next = std::move(head);
    head = std::move(tmp);
  }
  //    auto tmp = new_box<Node>(Node{std::move(_data)});

  auto pop() {
    if (not head) return;
    auto tmp = std::move(head);
    head = std::move(tmp->next);
  }

  auto size() const -> usize {
    usize size{};
    for (auto tmp = head.get(); tmp; tmp = tmp->next.get(), ++size)
      ;
    return size;
  }

  auto front() const -> const T & {
    assert(head.get());
    return head->data;
  }

  auto clean() -> void {
    while (head) head.reset(head->next.release());
  }

  template <typename OS>
  friend auto operator<<(OS &os, const ForwardList &rhs) -> OS & {
    os << '[' << ' ';
    for (auto tmp = rhs.head.get(); tmp; tmp = tmp->next.get())
      os << tmp->data << ", ";
    return os << ']';
  }
};
}  // namespace my
