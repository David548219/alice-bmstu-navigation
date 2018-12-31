// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_NAVIGATION_HPP_
#define INCLUDES_NAVIGATION_HPP_

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <stack>
#include <string>
#include <vector>
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
    DijkstraNode() {
      nodeWeight = std::numeric_limits<double>::infinity();
      isVisited = false;
      container = new nav::MapNode();
    }
    DijkstraNode(nav::MapNode* _container) {
      nodeWeight = std::numeric_limits<double>::infinity();
      isVisited = false;
      container = _container;
    }
    ~DijkstraNode() {}
  };
  class DijkstraBranch {
   public:
    DijkstraNode* node;
    double weight;
    nav::BranchType branchType;
    DijkstraBranch(DijkstraNode* _node,
                   double _weight,
                   nav::BranchType _branchType) {
      node = _node;
      weight = _weight;
      branchType = _branchType;
    }
  };
  std::vector<DijkstraNode*> nodes;

  DijkstraNode* FindNode(const std::string& _id);
  void AddNode(nav::MapNode* _container);
  void AddBranch(const std::string& _firstId,
                 nav::BranchType _firstBranchType,
                 const std::string& _secondId,
                 nav::BranchType _secondBranchType,
                 double _weight);
  const std::vector<DijkstraNode*>& GetNodes();
  void DeepCalculateMetrics(const std::string& _a, bool _isFirstCall = true);
  std::queue<DijkstraBranch*> AssembleRoute(const std::string& _a,
                                            const std::string& _b);
  void ResetDijkstraCalculationData();
  std::queue<DijkstraBranch*> PlotRoute(const std::string& _a,
                                        const std::string& _b);
};
}  // namespace nav
#endif  // INCLUDES_NAVIGATION_HPP_
