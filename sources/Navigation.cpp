// Copyright 2018 David david548219@gmail.com

#include "Navigation.hpp"
#include <algorithm>
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
                                   int _firstOrientation,
                                   const std::string& _secondId,
                                   int _secondOrientation,
                                   double _weight) {
  auto _nodeFirst = FindNode(_firstId);
  auto _nodeSecond = FindNode(_secondId);
  auto _branchFirst =
      new DijkstraBranch(_nodeSecond, _weight, _firstOrientation);
  auto _branchSecond =
      new DijkstraBranch(_nodeFirst, _weight, _secondOrientation);
  _branchFirst->reverse = _branchSecond;
  _branchSecond->reverse = _branchFirst;
  _nodeFirst->branches.push_back(_branchFirst);
  _nodeSecond->branches.push_back(_branchSecond);
}

const std::vector<nav::DijkstraGraph::DijkstraNode*>&
nav::DijkstraGraph::GetNodes() {
  return nodes;
}

void nav::DijkstraGraph::Purge() {
  for (DijkstraNode* _node : nodes) {
    for (DijkstraBranch* _branch : _node->branches) {
      delete _branch;
    }
    delete _node->container;
    delete _node;
  }
}

void nav::DijkstraGraph::DeepCalculateMetrics(const std::string& _a,
                                              bool _isFirstCall) {
  DijkstraNode* _anchor = FindNode(_a);
  if (_isFirstCall) {
    _anchor->nodeWeight = 0;
  }
  _anchor->isVisited = true;
  for (DijkstraBranch* _branch : _anchor->branches) {
    DijkstraNode* _current = _branch->destination;
    if (!_current->isVisited) {
      if (_current->nodeWeight > _anchor->nodeWeight + _branch->weight) {
        _current->nodeWeight = _anchor->nodeWeight + _branch->weight;
      }
    }
  }
  std::string _minWeightNodeID = _anchor->container->id;
  double _min = 0;
  for (const DijkstraNode* _node : nodes) {
    if (!_node->isVisited && (_node->nodeWeight < _min ||
                              _minWeightNodeID == _anchor->container->id)) {
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

std::deque<nav::DijkstraGraph::DijkstraBranch*>
nav::DijkstraGraph::AssembleRoute(const std::string& _a,
                                  const std::string& _b,
                                  bool _isFirstCall) {
  std::deque<DijkstraBranch*> _route;
  DijkstraNode* _current = FindNode(_b);
  double _routeRemainingLength = _current->nodeWeight;
  bool _noJunctionsMet = true;
  while (!DoubleEpsilonCompare(_routeRemainingLength, 0.0)) {
    for (DijkstraBranch* _branch : _current->branches) {
      if (DoubleEpsilonCompare(
              (_routeRemainingLength - _branch->weight),
              FindNode(_branch->destination->container->id)->nodeWeight)) {
        _routeRemainingLength -= _branch->weight;
        if (_branch->reverse->destination->container->isJunction ||
            ((_route.empty()) ||
             (_noJunctionsMet &&
              ((_route[0]->orientation + _branch->orientation) % 4) % 2 ==
                  1))) {
          if (_branch->destination->container->isJunction) {
            _noJunctionsMet = false;
          }
          _route.push_front(_branch->reverse);
        }
        _current = _branch->destination;
        break;
      }
    }
  }
  if (_isFirstCall) {
    for (DijkstraBranch* _branch :
         AssembleRoute(_a, _route[0]->destination->container->id, false)) {
      _route.push_front(_branch);
    }
    _route.pop_front();
  }
  return _route;
}

void nav::DijkstraGraph::ResetDijkstraCalculationData() {
  for (DijkstraNode* _node : nodes) {
    _node->isVisited = false;
    _node->nodeWeight = std::numeric_limits<double>::infinity();
  }
}

std::deque<nav::DijkstraGraph::DijkstraBranch*> nav::DijkstraGraph::PlotRoute(
    const std::string& _a,
    const std::string& _b) {
  DeepCalculateMetrics(_a);
  auto _route = AssembleRoute(_a, _b);
  ResetDijkstraCalculationData();
  return _route;
}
