// Copyright 2018 David david548219@gmail.com

#include "Conversion.hpp"
#include <exception>
#include <vector>

bool nav::TryGetIdFromAlias(nav::DijkstraGraph& _graph,
                            std::string _name,
                            std::string& _out) {
  for (DijkstraGraph::DijkstraNode* _node : _graph.GetNodes()) {
    for (const std::string& _str : _node->container->alias) {
      if (_str == _name) {
        _out = _node->container->id;
        return true;
      }
    }
  }
  return false;
}

int nav::OrientationShiftCW(int _orientation) {
  return (_orientation + 1) % 4;
}

int nav::OrientationShiftCCW(int _orientation) {
  return _orientation == 0 ? 3 : _orientation - 1;
}

nav::Direction nav::GetMoveDirection(
    const DijkstraGraph::DijkstraBranch* _current,
    const DijkstraGraph::DijkstraBranch* _prev) {
  if (_current == nullptr || _prev == nullptr) {
    throw std::invalid_argument("Null pointer exception");
  }
  if (_prev->reverse->orientation % 2 == _current->orientation % 2) {
    return Direction::Forward;
  } else if (_prev->reverse->orientation ==
             OrientationShiftCCW(_current->orientation)) {
    return Direction::Left;
  } else if (_prev->reverse->orientation ==
             OrientationShiftCW(_current->orientation)) {
    return Direction::Right;
  }
  return Direction::Null;
}

nav::Response nav::ResponseFromRoute(
    const std::deque<nav::DijkstraGraph::DijkstraBranch*>& _route) {
  Response _response = std::make_pair("", "Маршрут построен");
  for (size_t i = 0; i < _route.size(); i++) {
    std::string _currentStep = std::to_string(i + 1) + ": ";
    if (i == 0) {
      _currentStep = "Выйдите из кабинета";
    } else if (i == _route.size() - 1) {
      Direction _direction = GetMoveDirection(_route[i], _route[i - 1]);
      if (_direction == Direction::Forward) {
        _currentStep = "Идите прямо до точки назначения";
      } else if (_direction == Direction::Left) {
        _currentStep = "Идите прямо, точка назначения будет слева";
      } else if (_direction == Direction::Right) {
        _currentStep = "Идите прямо, точка назначения будет справа";
      }
    } else {
      Direction _direction = GetMoveDirection(_route[i], _route[i - 1]);
      if (_direction == Direction::Forward) {
        _currentStep = "Идите прямо";
      } else if (_direction == Direction::Left) {
        _currentStep = "Поверните налево";
      } else if (_direction == Direction::Right) {
        _currentStep = "Поверните направо";
      }
    }

    if (i != _route.size() - 1) {
      _currentStep += '\n';
    }
    _response.first += _currentStep;
  }
  return _response;
}
