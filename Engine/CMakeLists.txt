project (Vhagar)

file(GLOB Vhagar_SRC Source/*/*.cpp Source/*/*.glsl Source/*/*.hpp Include/*/*.hpp Source/*.hpp Source/*.cpp)

source_groups("Source Files" Source *.cpp *.hpp *.glsl)
source_groups("Header Files" Include Vhagar.hpp)

include_directories("Source")
include_directories("Include")

add_library(${PROJECT_NAME} ${Vhagar_SRC})
target_link_libraries(${PROJECT_NAME} ${DEP_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
  set(VHAGAR_COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS} -Wall")
endif()

set_target_properties (Vhagar PROPERTIES COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS}")

setup_pch(Vhagar_SRC Vhagar_PCH.hpp ${PROJECT_SOURCE_DIR}/Source/Vhagar_PCH.cpp ${PROJECT_SOURCE_DIR}/Source/Vhagar_PCH.hpp)

if(MSVC)
  target_link_libraries(Vhagar Ws2_32)
endif(MSVC)

add_subdirectory (Source)
#add_subdirectory (RenderingTest)
#add_subdirectory (test-game)
