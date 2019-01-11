// Copyright 2018 David david548219@gmail.com

#include "Conversion.hpp"
#include <exception>
#include <vector>

int nav::OrientationShiftCW(int _orientation) {
  return (_orientation + 1) % 4;
}

int nav::OrientationShiftCCW(int _orientation) {
  return _orientation == 0 ? 3 : _orientation - 1;
}

nav::Response nav::ResponseFromBranches(
    const nav::DijkstraGraph::DijkstraBranch* _current,
    const nav::DijkstraGraph::DijkstraBranch* _prev) {
  if (_prev == nullptr) {
    return std::make_pair(
        "��������� � " + _current->destination->container->id,
        "��������� � " + _current->destination->container->id);
  }
  if (_prev->reverse->orientation % 2 == _current->orientation % 2) {
    return std::make_pair("�����, � " + _current->destination->container->id,
                          "�����, � " + _current->destination->container->id);
  } else if (_prev->reverse->orientation ==
             OrientationShiftCCW(_current->orientation)) {
    return std::make_pair(
        "��������� ������, � " + _current->destination->container->id,
        "��������� ������, � " + _current->destination->container->id);
  } else if (_prev->reverse->orientation ==
             OrientationShiftCW(_current->orientation)) {
    return std::make_pair(
        "��������� �������, � " + _current->destination->container->id,
        "��������� �������, � " + _current->destination->container->id);
  }
  return std::make_pair("������", "������");
}
