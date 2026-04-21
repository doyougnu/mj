#ifndef J_UTILS_H
#define J_UTILS_H

#include <variant>
// TODO use Result
template <typename T> struct Ok;
template <typename E> struct Err;

template <typename T, typename E> using Result = std::variant<Err<E>, Ok<T>>;

template <typename T> struct Ok {
  T value;
  explicit Ok(T &&t) : value(std::move(t)) {};
};

template <typename E> struct Err {
  E err;
  explicit Err(E &&e) : err(std::move(e)) {};
};

#endif // J_UTILS_H

template <typename... Ts> struct cases : Ts... {
  using Ts::operator()...;
};
