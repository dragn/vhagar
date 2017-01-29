project(RockBandTycoon)

include_directories(../Engine/Include)
include_directories(../Engine/Source)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} ${DEP_LIBRARIES} Vhagar)
