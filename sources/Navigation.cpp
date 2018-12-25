// Copyright 2018 David david548219@gmail.com

#include "Navigation.hpp"
#include <cmath>
#include <exception>
#include <iostream>

nav::DijkstraGraph::DijkstraNode& nav::DijkstraGraph::FindNode(
    const std::string& _id) {
  for (DijkstraNode& _node : nodes) {
    if (_node.container.id == _id) {
      return _node;
    }
  }
  throw std::invalid_argument("Bad ID (Not present in database)");
}

void nav::DijkstraGraph::AddNode(Container& _container) {
  nodes.push_back(DijkstraNode(_container));
}

void nav::DijkstraGraph::AddBranch(const std::string& _first,
                                   const std::string& _second,
                                   double _weight) {
  FindNode(_first).neighbors.push_back(std::make_pair(_second, _weight));
  FindNode(_second).neighbors.push_back(std::make_pair(_first, _weight));
}

const std::vector<nav::DijkstraGraph::DijkstraNode>&
nav::DijkstraGraph::GetNodes() {
  return nodes;
}

void nav::DijkstraGraph::DeepCalculateMetrics(const std::string& _a,
                                              bool _isFirstCall) {
  DijkstraNode& _anchor = FindNode(_a);
  if (_isFirstCall) {
    _anchor.nodeWeight = 0;
  }
  _anchor.isVisited = true;
  for (const std::pair<std::string, double>& _neighbor : _anchor.neighbors) {
    DijkstraNode& _current = FindNode(_neighbor.first);
    if (!_current.isVisited) {
      if (_current.nodeWeight > _anchor.nodeWeight + _neighbor.second) {
        _current.nodeWeight = _anchor.nodeWeight + _neighbor.second;
      }
    }
  }
  std::string _minWeightNodeID = _anchor.container.id;
  double _min = 0;
  for (const DijkstraNode& _node : nodes) {
    if (!_node.isVisited &&
        (_node.nodeWeight < _min || _minWeightNodeID == _anchor.container.id)) {
      _min = _node.nodeWeight;
      _minWeightNodeID = _node.container.id;
    }
  }
  if (_minWeightNodeID != _anchor.container.id) {
    DeepCalculateMetrics(_minWeightNodeID, false);
  }
}

bool DoubleEpsilonCompare(double _a, double _b) {
  return abs((_a - _b)) < std::numeric_limits<double>::epsilon();
}

std::stack<nav::Container> nav::DijkstraGraph::AssembleRoute(
    const std::string& _a,
    const std::string& _b) {
  std::stack<Container> _route;
  DijkstraNode _current = FindNode(_b);
  _route.push(_current.container);
  double _routeRemainingLength = _current.nodeWeight;
  while (!DoubleEpsilonCompare(_routeRemainingLength, 0.0)) {
    for (const std::pair<std::string, double>& _neighbor : _current.neighbors) {
      if (DoubleEpsilonCompare((_routeRemainingLength - _neighbor.second),
                               FindNode(_neighbor.first).nodeWeight)) {
        _routeRemainingLength -= _neighbor.second;
        _current = FindNode(_neighbor.first);
        _route.push(_current.container);
        break;
      }
    }
  }
  return _route;
}

void nav::DijkstraGraph::ResetDijkstraCalculationData() {
  for (DijkstraNode& _node : nodes) {
    _node.isVisited = false;
    _node.nodeWeight = std::numeric_limits<double>::infinity();
  }
}

void nav::DijkstraGraph::PlotRoute(
    const std::string& _a,
    const std::string& _b,
    std::function<void(std::stack<Container>&)> _ExportRoute) {
  DeepCalculateMetrics(_a);
  auto _route = AssembleRoute(_a, _b);
  ResetDijkstraCalculationData();
  _ExportRoute(_route);
}
