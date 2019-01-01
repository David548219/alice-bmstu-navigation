// Copyright 2018 David david548219@gmail.com
#include <Request.hpp>
#include <Response.hpp>
#include <Skill.hpp>

#include "Interface.hpp"

void bmstu_navigation_callback(const Alice::Request& request,
                               Alice::Response& response) {
  response.SetText("Sample Text");
  response.SetTts("Sample Text");
  response.SetEndSession(true);
}

int main() {
  Skill NavigatorBmstu;
  NavigatorBmstu.SetCallback(bmstu_navigation_callback);
  NavigatorBmstu.Run();
  return 0;
}
