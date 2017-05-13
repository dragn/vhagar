project(Game)

include_directories(../Engine/Include)
include_directories(../Engine/Source)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} ${DEP_LIBRARIES} Vhagar)

copy_shared_libs(PhysX_SHARED_LIBRARIES_DEBUG PhysX_SHARED_LIBRARIES_RELEASE)
