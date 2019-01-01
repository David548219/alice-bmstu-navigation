// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_NAVIGATION_HPP_
#define INCLUDES_NAVIGATION_HPP_

#include <algorithm>
#include <deque>
#include <functional>
#include <limits>
#include <string>
#include "Storage.hpp"

namespace nav {
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
  const std::vector<DijkstraNode*>& GetNodes();
  void Purge();
  void DeepCalculateMetrics(const std::string& _a, bool _isFirstCall = true);
  std::deque<DijkstraBranch*> AssembleRoute(const std::string& _a,
                                            const std::string& _b);
  void ResetDijkstraCalculationData();
  std::deque<DijkstraBranch*> PlotRoute(const std::string& _a,
                                        const std::string& _b);
};
}  // namespace nav
#endif  // INCLUDES_NAVIGATION_HPP_
