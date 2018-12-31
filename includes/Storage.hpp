// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_STORAGE_HPP_
#define INCLUDES_STORAGE_HPP_

#include <algorithm>
#include <exception>
#include <functional>
#include <limits>
#include <random>
#include <stack>
#include <string>
#include <vector>

namespace nav {
class Container {
 public:
  std::string id;
};
using Response = std::pair<std::string, std::string>;
using Responses = std::vector<Response>;
class ResponseCollection {
 public:
  ResponseCollection(){};
  ResponseCollection(const Responses& _responces) { responses = _responces; };
  ResponseCollection(const Responses& _responces,
                     std::vector<ResponseCollection>& _collections) {
    responses = _responces;
    _collections.push_back(*this);
  };
  ~ResponseCollection(){};

  Response GetRandomResponse();

 private:
  Responses responses;  // Pair: First - Text; Second - TTS
};
enum BranchType { StartingPoint = 0, TurnLeft, TurnRight, GoAhead, GoBackwards, Destination };
class MapNode : public Container {
 public:
  MapNode() {
    id = "nil";
  }
  MapNode(const std::string& _id) {
    id = _id;
  }
  ~MapNode() {}
};
}  // namespace nav
#endif  // INCLUDES_STORAGE_HPP_
