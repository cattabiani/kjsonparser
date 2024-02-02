#pragma once

#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

// Helper type trait to identify std::string
template <typename T> struct is_string : std::false_type {};

template <> struct is_string<std::string> : std::true_type {};

// Helper to determine if a type is an iterable container
template <typename T, typename = void> struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                  decltype(std::declval<T>().end())>>
    : std::true_type {};

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename Iterable,
          typename = std::enable_if_t<!is_string<Iterable>::value &&
                                      is_iterable<Iterable>::value>>
std::ostream& operator<<(std::ostream& os, const Iterable& container) {
    os << '[';
    for (auto it = container.begin(); it != container.end();
         /* no increment here */) {
        os << *it;
        if (++it != container.end()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template <class T> bool is_close(T x0, T x1, T v) {
    return (x0 - v) <= x1 <= (x0 + v);
}