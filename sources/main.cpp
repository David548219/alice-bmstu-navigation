// Copyright 2018 David david548219@gmail.com

#include <Request.hpp>
#include <Response.hpp>
#include <Skill.hpp>

#include "Conversion.hpp"
#include "Database.hpp"
#include "Navigation.hpp"

std::vector<std::string> splitToLowerCase(const std::string& str, char sep) {
  std::vector<std::string> words;
  size_t pos = 0;
  size_t sepPos = 0;
  while ((sepPos = str.find(sep, pos)) != std::string::npos) {
    if (sepPos - pos != 0) {
      std::string _buffer = str.substr(pos, sepPos - pos);
      std::transform(_buffer.begin(), _buffer.end(), _buffer.begin(),
                     ::tolower);
      words.push_back(_buffer);
    }
    pos = sepPos + 1;
  }
  if (pos < str.size()) {
    words.push_back(str.substr(pos));
  }
  return words;
}

void bmstu_navigation_callback(const Alice::Request& _request,
                               Alice::Response& _response) {
  if (_request.Command() == "") {
    std::string _string = nav::PickRandomFromVector<std::string>(
        {"Приветствую, скажи откуда и куда тебя провести?",
         "Я могу привести провести в любое место, скажи только откуда и куда.",
         "В МГТУ столько интересных мест, скажи куда и откуда, я помогу найти "
         "дорогу.",
         "Уже заблудился? Скажи где ты и куда идти, попробую помочь."});
    _response.SetText(_string);
    _response.SetTts(_string);
    _response.SetEndSession(true);
    return;
  } else {
    std::vector<std::string> _tokens =
        splitToLowerCase(_request.Command(), ' ');
    std::string _from;
    std::string _to;
    for (int _i = 0; _i < _tokens.size(); ++_i) {
      if (_tokens[_i] == "из" && _i < _tokens.size() - 1) {
        _from = _tokens[_i + 1];
      } else if (_tokens[_i] == "в" && _i < _tokens.size() - 1) {
        _to = _tokens[_i + 1];
      }
    }
    nav::DijkstraGraph _graph;
    nav::PopulateGraph(_graph, "ulk");
    if (!nav::TryGetIdFromAlias(_graph, _from, _from) ||
        !nav::TryGetIdFromAlias(_graph, _to, _to)) {
      std::string _string = nav::PickRandomFromVector<std::string>(
          {"Не припоминаю таких мест, вы не ошиблись?",
           "А вы точно правильно указали точки?"
           "Такого на карте нет!",
           "Кажется произошла ошибка, таких точек нет."});
      _response.SetText(_string);
      _response.SetTts(_string);
      _response.SetEndSession(true);
      return;
    }
    nav::Response _r = nav::ResponseFromRoute(_graph.PlotRoute(_from, _to));
    _response.SetText(_r.first);
    _response.SetTts(_r.second);
    _response.SetEndSession(true);
    return;
  }
}

int main() {
  Skill NavigatorBmstu;
  NavigatorBmstu.SetCallback(bmstu_navigation_callback);
  NavigatorBmstu.Run();
  return 0;
}
