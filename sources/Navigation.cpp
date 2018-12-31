// Copyright 2018 David david548219@gmail.com

#include "Navigation.hpp"
#include <cmath>
#include <exception>
#include <utility>

nav::DijkstraGraph::DijkstraNode* nav::DijkstraGraph::FindNode(
    const std::string& _id) {
  for (DijkstraNode* _node : nodes) {
    if (_node->container->id == _id) {
      return _node;
    }
  }
  throw std::invalid_argument("Bad ID (Not present in database)");
}

void nav::DijkstraGraph::AddNode(nav::MapNode* _container) {
  nodes.push_back(new DijkstraNode(_container));
}

void nav::DijkstraGraph::AddBranch(const std::string& _firstId,
                                   nav::BranchType _firstBranchType,
                                   const std::string& _secondId,
                                   nav::BranchType _secondBranchType,
                                   double _weight) {
  auto _nodeFirst = FindNode(_firstId);
  auto _nodeSecond = FindNode(_secondId);
  _nodeFirst->branches.push_back(
      new DijkstraBranch(_nodeSecond, _weight, _firstBranchType));
  _nodeSecond->branches.push_back(
      new DijkstraBranch(_nodeFirst, _weight, _secondBranchType));
}

const std::vector<nav::DijkstraGraph::DijkstraNode*>&
nav::DijkstraGraph::GetNodes() {
  return nodes;
}

void nav::DijkstraGraph::DeepCalculateMetrics(const std::string& _a,
                                              bool _isFirstCall) {
  DijkstraNode* _anchor = FindNode(_a);
  if (_isFirstCall) {
    _anchor->nodeWeight = 0;
  }
  _anchor->isVisited = true;
  for (DijkstraBranch* _branch : _anchor->branches) {
    DijkstraNode* _current = _branch->node;
    if (!_current->isVisited) {
      if (_current->nodeWeight > _anchor->nodeWeight + _branch->weight) {
        _current->nodeWeight = _anchor->nodeWeight + _branch->weight;
      }
    }
  }
  std::string _minWeightNodeID = _anchor->container->id;
  double _min = 0;
  for (const DijkstraNode* _node : nodes) {
    if (!_node->isVisited &&
        (_node->nodeWeight < _min || _minWeightNodeID == _anchor->container->id)) {
      _min = _node->nodeWeight;
      _minWeightNodeID = _node->container->id;
    }
  }
  if (_minWeightNodeID != _anchor->container->id) {
    DeepCalculateMetrics(_minWeightNodeID, false);
  }
}

bool DoubleEpsilonCompare(double _a, double _b) {
  return abs((_a - _b)) < std::numeric_limits<double>::epsilon();
}

std::queue<nav::DijkstraGraph::DijkstraBranch*>
nav::DijkstraGraph::AssembleRoute(const std::string& _a,
                                  const std::string& _b) {
  std::stack<DijkstraBranch*> _route;
  DijkstraNode* _current = FindNode(_b);
  _route.push(new DijkstraBranch(_current, 0.0, BranchType::Destination));
  double _routeRemainingLength = _current->nodeWeight;
  while (!DoubleEpsilonCompare(_routeRemainingLength, 0.0)) {
    for (DijkstraBranch* _branch : _current->branches) {
      if (DoubleEpsilonCompare((_routeRemainingLength - _branch->weight),
                               FindNode(_branch->node->container->id)->nodeWeight)) {
        _routeRemainingLength -= _branch->weight;
        _route.push(_branch);
        _current = FindNode(_branch->node->container->id);
        break;
      }
    }
  }
  std::queue<DijkstraBranch*> _parsedRoute;
  double _lastBranchWeight = 0.0;
  while (!_route.empty()) {
    DijkstraBranch* _buffer = _route.top();
    _route.pop();
    std::swap(_buffer->weight, _lastBranchWeight);
    if (_buffer->node->container->id == _a) {
      _buffer->branchType = BranchType::StartingPoint;
    }
    _parsedRoute.push(_buffer);
  }
  return _parsedRoute;
}

void nav::DijkstraGraph::ResetDijkstraCalculationData() {
  for (DijkstraNode* _node : nodes) {
    _node->isVisited = false;
    _node->nodeWeight = std::numeric_limits<double>::infinity();
  }
}

std::queue<nav::DijkstraGraph::DijkstraBranch*> nav::DijkstraGraph::PlotRoute(
    const std::string& _a,
    const std::string& _b) {
  DeepCalculateMetrics(_a);
  auto _route = AssembleRoute(_a, _b);
  ResetDijkstraCalculationData();
  return _route;
}
