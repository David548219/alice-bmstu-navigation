// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_NAVIGATION_HPP_
#define INCLUDES_NAVIGATION_HPP_

#include <algorithm>
#include <deque>
#include <functional>
#include <limits>
#include <string>
#include <vector>

namespace nav {
class MapNode {
 public:
  std::string id;
  std::vector<std::string> alias;  // alias[0] is the name of MapNode
  bool isVisibleInSearch;
  bool isJunction;
  MapNode(const std::string& _id,
          const std::vector<std::string>& _alias,
          bool _isJunction = false,
          bool _isVisibleInSearch = true) {
    id = _id;
    alias = _alias;
    isVisibleInSearch = _isVisibleInSearch;
    isJunction = _isJunction;
  }
  MapNode(const std::string& _id,
          bool _isJunction = true,
          bool _isVisibleInSearch = false) {
    id = _id;
    alias = {_id};
    isVisibleInSearch = _isVisibleInSearch;
    isJunction = _isJunction;
  }
  ~MapNode() {}
};
class DijkstraGraph {
 public:
  class DijkstraNode;
  class DijkstraBranch;

  class DijkstraNode {
   public:
    double nodeWeight;
    bool isVisited;
    std::vector<DijkstraBranch*> branches;
    nav::MapNode* container;
    DijkstraNode(nav::MapNode* _container) {
      nodeWeight = std::numeric_limits<double>::infinity();
      isVisited = false;
      container = _container;
    }
    ~DijkstraNode() {}
  };
  class DijkstraBranch {
   public:
    DijkstraNode* destination;
    DijkstraBranch* reverse;
    double weight;
    int orientation;
    DijkstraBranch(DijkstraNode* _destination,
                   double _weight,
                   int _orientation) {
      destination = _destination;
      reverse = nullptr;
      weight = _weight;
      orientation = _orientation;
    }
  };
  std::vector<DijkstraNode*> nodes;

  DijkstraGraph() {}
  ~DijkstraGraph() { (*this).Purge(); };

  DijkstraNode* FindNode(const std::string& _id);
  void AddNode(nav::MapNode* _container);
  void AddBranch(const std::string& _firstId,
                 int _firstOrientation,
                 const std::string& _secondId,
                 int _secondOrientation,
                 double _weight);
  const std::vector<DijkstraNode*>& GetNodes() { return nodes; }
  void DeepCalculateMetrics(const std::string& _a, bool _isFirstCall = true);
  std::deque<DijkstraBranch*> AssembleRoute(const std::string& _a,
                                            const std::string& _b, bool _isFirstCall = true);
  void ResetDijkstraCalculationData();
  std::deque<DijkstraBranch*> PlotRoute(const std::string& _a,
                                        const std::string& _b);

 private:
  void Purge();
};
}  // namespace nav
#endif  // INCLUDES_NAVIGATION_HPP_
