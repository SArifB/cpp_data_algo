#pragma once
#include <stdio.h>

#include <format>
#include <iostream>
#include <string>

#include "array.hpp"
#include "cont.hpp"
#include "span.hpp"
#include "vector.hpp"

namespace my {

 int print(const my::Vector<char> charr) {
  return fwrite(charr.begin(), sizeof(char), charr.size(), stdout);
}

 int print(my::Span<char, -1ULL> charr) {
  return fwrite(charr.begin(), sizeof(char), charr.size(), stdout);
}

 int print(const char *ch, usize len) {
   return fwrite(ch, sizeof(char), len, stdout);
 }

template <typename... Args>
constexpr auto logln(Args... args) -> void {
  auto buf = Array<char, (sizeof...(Args) * 3) + 1>{};
  constexpr auto tmp = Array<char, 3>{'{', '}', ' '};
  for (usize i{}; i != buf.size(); ++i) buf[i] = tmp[i % 3];
  buf.back() = '\0';
  puts(std::vformat(buf.begin(), std::make_format_args(args...)).c_str());
}

template <typename... Args>
constexpr auto log(Args... args) -> void {
  auto buf = Array<char, (sizeof...(Args) * 3) + 1>{};
  constexpr auto tmp = Array<char, 3>{'{', '}', ' '};
  for (usize i{}; i != buf.size(); ++i) buf[i] = tmp[i % 3];
  buf.back() = '\0';
  fputs(std::vformat(buf.begin(), std::make_format_args(args...)).c_str(),
        stdout);
}

}  // namespace my

// My Array
 template <typename T, usize N>
 struct std::formatter<my::Array<T, N>> : std::formatter<std::string> {
   template <class Ctx>
   constexpr auto format(my::Array<T, N> arr, Ctx &ctx) const {
     std::string str{"["};
     for (const auto &value : arr)
       str.append(std::format("{}, ", value));
     str.pop_back();
     str.pop_back();
     str.append("]");
     return std::formatter<std::string>::format(str, ctx);
   }
 };

 template <usize N>
 struct std::formatter<my::Array<char, N>> : std::formatter<std::string> {
   template <class Ctx>
   constexpr auto format(my::Array<char, N> arr, Ctx &ctx) const {
     std::string str{"\""};
     for (const auto &x : arr)
       str.append(std::format("{}", x));
     str.append("\"");
     return std::formatter<std::string>::format(str, ctx);
   }
 };

// My Vector
 template <typename T>
 struct std::formatter<my::Vector<T>> : std::formatter<std::string> {
   template <class Ctx>
   constexpr auto format(my::Vector<T> arr, Ctx &ctx) const {
     std::string str{"["};
     for (const auto &value : arr)
       str.append(std::format("{}, ", value));
     str.pop_back();
     str.pop_back();
     str.append("]");
     return std::formatter<std::string>::format(str, ctx);
   }
 };

 template <>
 struct std::formatter<my::Vector<char>> : std::formatter<std::string> {
   template <class Ctx>
   constexpr auto format(my::Vector<char> arr, Ctx &ctx) const {
     std::string str{"\""};
     for (const auto &x : arr)
       str.append(std::format("{}", x));
     str.append("\"");
     return std::formatter<std::string>::format(str, ctx);
   }
 };

// My Span
 template <typename T, usize N>
 struct std::formatter<my::Span<T, N>> : std::formatter<std::string> {
   template <class Ctx>
   constexpr auto format(my::Span<T, N> arr, Ctx &ctx) const {
     std::string str{"["};
     for (const auto &value : arr)
       str.append(std::format("{}, ", value));
     str.pop_back();
     str.pop_back();
     str.append("]");
     return std::formatter<std::string>::format(str, ctx);
   }
 };

 template <usize N>
 struct std::formatter<my::Span<char, N>> : std::formatter<std::string> {
   template <class Ctx>
   constexpr auto format(my::Span<char, N> arr, Ctx &ctx) const {
     std::string str{"\""};
     for (const auto &x : arr)
       str.append(std::format("{}", x));
     str.append("\"");
     return std::formatter<std::string>::format(str, ctx);
   }
 };
