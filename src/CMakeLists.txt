file(GLOB GameEngine_SRC "**/*.cpp")

add_executable (GameEngine main.cpp ${GameEngine_SRC})
set_target_properties (GameEngine PROPERTIES COMPILE_FLAGS "--std=c++11")
target_link_libraries (GameEngine SDL2 glog GLESv2)
