// Copyright 2018 David david548219@gmail.com

#ifndef INCLUDES_NAVIGATION_HPP_
#define INCLUDES_NAVIGATION_HPP_

#include <algorithm>
#include <functional>
#include <limits>
#include <stack>
#include <string>
#include <vector>

namespace nav {
class Container {
 public:
  std::string id;
  int value;
  Container() {
    id = "nil";
    value = -1;
  }
  Container(const std::string& _id, int _value) {
    id = _id;
    value = _value;
  }
  ~Container() {}
};
class DijkstraGraph {
 public:
  class DijkstraNode {
   public:
    double nodeWeight;
    bool isVisited;
    std::vector<std::pair<std::string, double>> neighbors;
    Container container;
    DijkstraNode() {
      nodeWeight = std::numeric_limits<double>::infinity();
      isVisited = false;
      container = Container();
    }
    DijkstraNode(Container& _container) {
      nodeWeight = std::numeric_limits<double>::infinity();
      isVisited = false;
      container = _container;
    }
    ~DijkstraNode() {}
  };
  std::vector<DijkstraNode> nodes;

  DijkstraNode& FindNode(const std::string& _id);
  void AddNode(Container& _origin);
  void AddBranch(const std::string& first,
                 const std::string& second,
                 double weight);
  const std::vector<DijkstraNode>& GetNodes();
  void DeepCalculateMetrics(const std::string& _a, bool _isFirstCall = true);
  std::stack<Container> AssembleRoute(const std::string& _a,
                                      const std::string& _b);
  void ResetDijkstraCalculationData();
  void PlotRoute(const std::string& _a,
                 const std::string& _b,
                 std::function<void(std::stack<Container>&)> _ExportRoute =
                     [](std::stack<Container>&) {});
};
}  // namespace nav
#endif  // INCLUDES_NAVIGATION_HPP_
