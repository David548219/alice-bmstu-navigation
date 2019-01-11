// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_CONVERSION_HPP_
#define INCLUDES_CONVERSION_HPP_

#include <deque>
#include <random>
#include <string>
#include "Navigation.hpp"

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
bool TryGetIdFromAlias(nav::DijkstraGraph& _graph,
                       std::string _name,
                       std::string& _out);
using Response =
    std::pair<std::string, std::string>;  // Pair: First - Text; Second - TTS
int OrientationShiftCW(int _orientation);
int OrientationShiftCCW(int _orientation);
enum Direction { Null = 0, Forward, Left, Right };
Direction GetMoveDirection(const nav::DijkstraGraph::DijkstraBranch* _current,
                           const nav::DijkstraGraph::DijkstraBranch* _prev);
Response ResponseFromRoute(
    const std::deque<nav::DijkstraGraph::DijkstraBranch*>& _route);
}  // namespace nav
#endif  // INCLUDES_CONVERSION_HPP_
