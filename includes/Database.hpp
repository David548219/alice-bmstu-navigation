// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_DATABASE_HPP_
#define INCLUDES_DATABASE_HPP_

#include "Navigation.hpp"

namespace nav {
void PopulateGraph(nav::DijkstraGraph& _graph, const std::string& _mapId);
}  // namespace nav
#endif  // INCLUDES_DATABASE_HPP_
