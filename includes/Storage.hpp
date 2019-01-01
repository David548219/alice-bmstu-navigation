// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_STORAGE_HPP_
#define INCLUDES_STORAGE_HPP_

#include <string>
#include <vector>

namespace nav {
class Container {
 public:
  std::string id;
};
using Response =
    std::pair<std::string, std::string>;  // Pair: First - Text; Second - TTS
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
  Responses responses;
};
class MapNode : public Container {
 public:
  MapNode(const std::string& _id) { id = _id; }
  ~MapNode() {}
};
}  // namespace nav
#endif  // INCLUDES_STORAGE_HPP_
