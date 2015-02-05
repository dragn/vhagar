file(GLOB Vhagar_SRC "Renderer/*.cpp" "RenderingObject/*.cpp")

add_library (Vhagar ${Vhagar_SRC})
target_link_libraries (Vhagar ${GLEW_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
  set(VHAGAR_COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS} -Wall")
endif()

set_target_properties (Vhagar PROPERTIES COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS}")

# Bundle shaders

set(shaders_src "Shaders/Shaders.hpp")

file(GLOB ShaderFiles "Shaders/*.glsl")

file(WRITE ${shaders_src} "#pragma once
#include<map>
#include<string>
namespace Vhagar {
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
