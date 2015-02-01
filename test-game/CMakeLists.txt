file(GLOB TestGame_SRC "**/*.cpp")

add_executable(TestGame test-game.cpp ${TestGame_SRC})
target_link_libraries (TestGame ${DEP_LIBRARIES})

target_compile_features(TestGame PRIVATE cxx_alias_templates)
target_compile_features(TestGame PRIVATE cxx_auto_type)
target_compile_features(TestGame PRIVATE cxx_nullptr)

include_directories("../engine")
find_library(GameEngine_LIB GameEngine HINTS "../engine")
target_link_libraries(TestGame ${GameEngine_LIB})
