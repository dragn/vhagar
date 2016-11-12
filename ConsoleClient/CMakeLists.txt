project(ConsoleClient)

include_directories(../Engine/Include)
include_directories(../Engine/Source)

add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries (${PROJECT_NAME} ${DEP_LIBRARIES} Vhagar)
