#pragma once

#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

// Helper type trait to identify std::pair
template <typename T> struct is_pair : std::false_type {};

template <typename T1, typename T2>
struct is_pair<std::pair<T1, T2>> : std::true_type {};

// Helper type trait to identify std::string
template <typename T> struct is_string : std::false_type {};

template <> struct is_string<std::string> : std::true_type {};

// Helper to determine if a type is an iterable container
template <typename T, typename = void> struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                  decltype(std::declval<T>().end())>>
    : std::true_type {};

inline std::ostream &operator<<(std::ostream &os, const std::string &s) {
    os.put('\"');
    os.write(s.c_str(), s.size());
    os.put('\"');
    return os;
}

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
    os << p.first << ": " << p.second;
    return os;
}

template <typename Iterable,
          typename = std::enable_if_t<!is_string<Iterable>::value &&
                                      is_iterable<Iterable>::value>>
std::ostream &operator<<(std::ostream &os, const Iterable &container) {
    constexpr bool is_pair_value =
        (is_pair<typename Iterable::value_type>::value);
    os.put(is_pair_value ? '{' : '[');
    for (auto it = container.begin(); it != container.end();
         /* no increment here */) {
        os << *it;
        if (++it != container.end()) {
            os << ", ";
        }
    }
    os.put(is_pair_value ? '}' : ']');
    return os;
}

template <typename T, typename... Ts>
std::ostream &operator<<(std::ostream &os, const std::variant<T, Ts...> &v) {
    std::visit([&os](auto &&arg) { os << arg; }, v);
    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::shared_ptr<T> &v) {
    if (v) {
        os << *v;
    } else {
        os << "nullptr";
    }
    return os;
}

template <class T> bool is_close(T x0, T x1, T v) {
    return (x0 - v) <= x1 <= (x0 + v);
}