project (Vhagar)

file(GLOB_RECURSE VhagarModules_SRC Source/Modules/*.cpp Source/Modules/*.hpp)
file(GLOB_RECURSE VhagarCore_SRC Source/Core/*.cpp Source/Core/*.hpp)
file(GLOB_RECURSE VhagarUtils_SRC Source/Utils/*.cpp Source/Utils/*.hpp)

source_groups("Source Files" Source *.cpp *.hpp *.glsl)
source_groups("Header Files" Include Vhagar.hpp)

include_directories("Source")
include_directories("Include")

add_library(VhUtils ${VhagarUtils_SRC})
target_link_libraries(VhUtils ${DEP_LIBRARIES})

add_library(VhCore ${VhagarCore_SRC})
target_link_libraries(VhCore VhUtils ${DEP_LIBRARIES})

add_library(VhModules ${VhagarModules_SRC})
target_link_libraries(VhModules VhCore VhUtils ${DEP_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
  set(VHAGAR_COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS} -Wall")
endif()

set_target_properties(VhModules PROPERTIES COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS}")
set_target_properties(VhCore PROPERTIES COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS}")
set_target_properties(VhUtils PROPERTIES COMPILE_FLAGS "${VHAGAR_COMPILE_FLAGS}")

if(MSVC)
  target_link_libraries(VhCore Ws2_32)
  target_link_libraries(VhModules Ws2_32)
endif(MSVC)

add_subdirectory (Source)
