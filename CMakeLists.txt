cmake_minimum_required(VERSION 3.0)
project(Vhagar)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(3RDPARTY "../vhagar-deps" CACHE STRING "3rd party libraries folder")

include(Utils)

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glog 0.3.4 REQUIRED)
find_package(GLM REQUIRED)
find_package(GL)
find_package(GLEW REQUIRED)
find_package(PhysX)

# Setup dependencies
if (SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIR})
  LIST(APPEND DEP_LIBRARIES ${SDL2_LIBRARY})
endif (SDL2_FOUND)

if (GLM_FOUND)
  include_directories(${GLM_INCLUDE_DIR})
endif (GLM_FOUND)

if (SDL2_IMAGE_FOUND)
  include_directories(${SDL2_IMAGE_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${SDL2_IMAGE_LIBRARY}")
endif (SDL2_IMAGE_FOUND)

if (SDL2_TTF_FOUND)
  include_directories(${SDL2_TTF_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${SDL2_TTF_LIBRARY}")
endif (SDL2_TTF_FOUND)

if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${GLEW_LIBRARIES}")
endif()

if (GL_FOUND)
  include_directories(${GL_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${GL_gl_LIBRARY}")
endif()

if (glog_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_GLOG")
  list(APPEND DEP_LIBRARIES glog::glog)
endif (glog_FOUND)

if (PhysX_FOUND)
  include_directories(${PhysX_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${PhysX_LIBRARIES}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_PHYSX")
endif (PhysX_FOUND)

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPPLE")
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif()

if (MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt")
  list(APPEND DEP_LIBRARIES "opengl32")
  list(APPEND DEP_LIBRARIES ${SDL2MAIN_LIBRARY} $<$<CONFIG:Debug>:msvcrtd> $<$<CONFIG:Release>:msvcrt>)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_subdirectory (Engine)
add_subdirectory (ConsoleClient)
