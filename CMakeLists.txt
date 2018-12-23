cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include("tools/alice-skills-lib/tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.31.tar.gz"
  SHA1 "3d94b8522541c4c8eaac606f4c47d09322a8019c"
)

project(alice)
string(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-command-line-argument -Wnon-virtual-dtor -pedantic -pthread")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

hunter_add_package(Beast)
find_package(Beast CONFIG REQUIRED)

add_executable(alice_tests
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Request.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Meta.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Session.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Response.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Button.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Card.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/ButtonPicture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/tests/request_unittests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/tests/response_unittests.cpp
)

add_executable(websocket-chat-server
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Request.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Meta.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Session.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Response.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Button.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/Card.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/source/ButtonPicture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

target_include_directories(alice_tests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/includes>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
)
target_include_directories(websocket-chat-server PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools/alice-skills-lib/includes>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
)

target_link_libraries(alice_tests GTest::main nlohmann_json::nlohmann_json Boost::boost Boost::system)
target_link_libraries(websocket-chat-server nlohmann_json::nlohmann_json Boost::boost Boost::system)

enable_testing()
add_test(NAME tests COMMAND alice_tests)
