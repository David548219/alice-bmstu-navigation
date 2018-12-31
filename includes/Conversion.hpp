// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_CONVERSION_HPP_
#define INCLUDES_CONVERSION_HPP_

#include "Storage.hpp"
#include <string>

namespace nav {
using Response = std::pair<std::string, std::string>;
Response ResponseFromMapNodes(const nav::MapNode& _origin,
                              const nav::MapNode& _destination);
}  // namespace nav
#endif  // INCLUDES_CONVERSION_HPP_
