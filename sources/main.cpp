// Copyright 2018 David david548219@gmail.com

#include <Request.hpp>
#include <Response.hpp>
#include <Skill.hpp>

#include "Conversion.hpp"
#include "Database.hpp"
#include "Navigation.hpp"

std::vector<std::string> Split(const std::string& str, char sep) {
  std::vector<std::string> words;
  size_t pos = 0;
  size_t sepPos = 0;
  while ((sepPos = str.find(sep, pos)) != std::string::npos) {
    if (sepPos - pos != 0) {
      words.push_back(str.substr(pos, sepPos - pos));
    }
    pos = sepPos + 1;
  }
  if (pos < str.size()) {
    words.push_back(str.substr(pos));
  }
  return words;
}

void ToLowerCase(std::vector<std::string>& _vec) {
  for (auto& _str : _vec) {
    std::transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
  }
}

void bmstu_navigation_callback(const Alice::Request& _request,
                               Alice::Response& _response) {
  if (_request.Command().empty()) {
    std::string _string = nav::PickRandomFromVector<std::string>(
        {"Приветствую, скажи откуда и куда тебя провести?",
         "Я могу привести провести в любое место, скажи только откуда и куда.",
         "В МГТУ столько интересных мест, скажи куда и откуда, я помогу найти "
         "дорогу.",
         "Уже заблудился? Скажи где ты и куда идти, попробую помочь."});
    _string += "\n(Пример поиска: из 601 в 610)";
    _response.SetText(_string);
    _response.SetTts(_string);
    _response.SetEndSession(false);
    return;
  }
  std::vector<std::string> _tokens = Split(_request.Command(), ' ');
  ToLowerCase(_tokens);
  std::string _from;
  std::string _to;
  for (int _i = 0; _i < _tokens.size(); ++_i) {
    if (_tokens[_i] == "из" && _i < _tokens.size() - 1) {
      _from = _tokens[_i + 1];
    } else if (_tokens[_i] == "в" && _i < _tokens.size() - 1) {
      _to = _tokens[_i + 1];
    }
  }
  if (_from.empty() && _to.empty()) {
    std::string _string = nav::PickRandomFromVector<std::string>(
        {"Не знаю к чему это вы, мы тут вроде маршруты строим?",
         "Я вас не поняла.", "Поробуйте ещё раз, это не сработало."});
    _response.SetText(_string);
    _response.SetTts(_string);
    _response.SetEndSession(false);
    return;
  } else if (_from.empty()) {
    std::string _string = nav::PickRandomFromVector<std::string>(
        {"Сложно прокладывать маршруты без начальной точки!",
         "Вы не указали начальную точку.", "А откуда мы начинаем путь?"});
    _response.SetText(_string);
    _response.SetTts(_string);
    _response.SetEndSession(false);
    return;
  } else if (_to.empty()) {
    std::string _string = nav::PickRandomFromVector<std::string>(
        {"У всего есть конец, а у вашего пути нет! В следующий раз укажите "
         "его.",
         "Вы забыли указать конечную точку.",
         "Без конечной точки я не смогу вам помочь."});
    _response.SetText(_string);
    _response.SetTts(_string);
    _response.SetEndSession(false);
    return;
  }
  nav::DijkstraGraph _graph;
  nav::PopulateGraph(_graph, "ulk");
  if (!nav::TryGetIdFromAlias(_graph, _from, _from) ||
      !nav::TryGetIdFromAlias(_graph, _to, _to)) {
    std::string _string = nav::PickRandomFromVector<std::string>(
        {"Не припоминаю таких мест, вы не ошиблись?",
         "А вы точно правильно указали точки?", "Такого на карте нет!",
         "Кажется произошла ошибка, таких точек нет."});
    _response.SetText(_string);
    _response.SetTts(_string);
    _response.SetEndSession(false);
    return;
  }
  nav::Response _r = nav::ResponseFromRoute(_graph.PlotRoute(_from, _to));
  _response.SetText(_r.first);
  _response.SetTts(_r.second);
  _response.SetEndSession(true);
  return;
}

int main() {
  Skill NavigatorBmstu;
  NavigatorBmstu.SetCallback(bmstu_navigation_callback);
  NavigatorBmstu.Run();
  return 0;
}
