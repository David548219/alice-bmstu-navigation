// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_CONVERSION_HPP_
#define INCLUDES_CONVERSION_HPP_

#include "Navigation.hpp"
#include <string>
#include <random>

namespace nav {
template <class T>
T PickRandomFromVector(std::vector<T> _v) {
  if (_v.empty()) {
    return T();
  }
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> dist(0, _v.size() - 1);
  return _v[dist(rd)];
}
using Response =
    std::pair<std::string, std::string>;  // Pair: First - Text; Second - TTS
Response ResponseFromBranches(
    const nav::DijkstraGraph::DijkstraBranch* _current,
    const nav::DijkstraGraph::DijkstraBranch* _prev = nullptr);
}  // namespace nav
#endif  // INCLUDES_CONVERSION_HPP_
