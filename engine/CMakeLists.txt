file(GLOB GameEngine_SRC "**/*.cpp")

add_library (GameEngine main.cpp ${GameEngine_SRC})
target_link_libraries (GameEngine ${DEP_LIBRARIES})

target_compile_features(GameEngine PRIVATE cxx_alias_templates)
target_compile_features(GameEngine PRIVATE cxx_auto_type)
target_compile_features(GameEngine PRIVATE cxx_nullptr)

if (CMAKE_COMPILER_IS_GNUCXX)
  set(GAMEENGINE_COMPILE_FLAGS "${GAMEENGINE_COMPILE_FLAGS} -Wall")
endif()

set_target_properties (GameEngine PROPERTIES COMPILE_FLAGS ${GAMEENGINE_COMPILE_FLAGS})

# Bundle shaders

set(shaders_src "shaders/Shaders.hpp")

file(GLOB ShaderFiles "shaders/*.glsl")

file(WRITE ${shaders_src} "#pragma once
#include<map>
#include<string>
namespace GameEngine {
  const std::map<std::string, std::string> Shaders = {")

foreach(shader ${ShaderFiles})
  file(READ ${shader} shader_content)

  # remove comments
  string(REGEX REPLACE "//[^\n]*\n" "" shader_content "${shader_content}")

  # remove extra spaces
  string(REGEX REPLACE " +" " " shader_content "${shader_content}")

  # remove line-breaks
  string(REPLACE "\n" "\\n" shader_content "${shader_content}")

  # remove path from file name
  string(REGEX REPLACE "^.*[/\\]" "" shader "${shader}")

  file(APPEND ${shaders_src} "{\"${shader}\",\"${shader_content}\"},")
endforeach(shader)

file(APPEND ${shaders_src} "};\n}")
