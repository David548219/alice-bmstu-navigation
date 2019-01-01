// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_CONVERSION_HPP_
#define INCLUDES_CONVERSION_HPP_

#include "Storage.hpp"
#include "Navigation.hpp"
#include <string>

namespace nav {
using Response = std::pair<std::string, std::string>;
Response ResponseFromBranches(const nav::DijkstraGraph::DijkstraBranch* _current, const nav::DijkstraGraph::DijkstraBranch* _prev = nullptr);
}  // namespace nav
#endif  // INCLUDES_CONVERSION_HPP_
