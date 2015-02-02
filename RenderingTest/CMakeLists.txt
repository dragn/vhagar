include_directories("../Vhagar")

add_executable (TestProgram TestProgram.cpp)
target_link_libraries (TestProgram ${DEP_LIBRARIES})
target_link_libraries (TestProgram "Vhagar")
