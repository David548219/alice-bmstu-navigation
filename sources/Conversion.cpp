// Copyright 2018 David david548219@gmail.com

#include "Conversion.hpp"
#include <cmath>

bool IsInEpsilonNeighborhoodInt1(int _number,
                                 int _anchor,
                                 unsigned int _epsilon = 5u) {
  return abs(_number - _anchor) < _epsilon;
}

nav::Response nav::ResponseFromMapNodes(const nav::MapNode& _origin,
                                        const nav::MapNode& _destination) {
  return Response();
}
