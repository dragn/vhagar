project(RockBandTycoon)

include_directories(Source)
include_directories(../Engine/Include)
include_directories(../Engine/Source)

file(GLOB_RECURSE SOURCES Source/*.cpp Source/*.hpp)

source_groups("Source Files" Source *.cpp *.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${DEP_LIBRARIES} Vhagar)

setup_pch(SOURCES PCH.hpp Source/PCH.cpp ${PROJECT_SOURCE_DIR}/Source/PCH.hpp)