// Copyright 2018 David david548219@gmail.com

#include "Storage.hpp"

nav::Response nav::ResponseCollection::GetRandomResponse() {
  if (responses.empty()) {
    throw std::exception("Can not get response from empty collection");
  }
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> dist(0, responses.size() - 1);
  return responses[dist(rd)];
}
