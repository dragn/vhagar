file(GLOB GameEngine_SRC "**/*.cpp")

add_library (GameEngine main.cpp ${GameEngine_SRC})
target_link_libraries (GameEngine ${DEP_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
  set(GAMEENGINE_COMPILE_FLAGS "${GAMEENGINE_COMPILE_FLAGS} -Wall")
endif()

set_target_properties (GameEngine PROPERTIES COMPILE_FLAGS ${GAMEENGINE_COMPILE_FLAGS})
