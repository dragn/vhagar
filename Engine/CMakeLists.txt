cmake_minimum_required (VERSION 3.0)
project (Vhagar)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(Utils)

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(Glog)
find_package(GLM REQUIRED)
find_package(GL)
find_package(GLEW REQUIRED)

# Setup dependencies
if (SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIR})
  LIST(APPEND DEP_LIBRARIES ${SDL2_LIBRARY})
endif (SDL2_FOUND)

if (GLOG_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_GLOG")
  include_directories(${GLOG_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${GLOG_LIBRARY}")
endif (GLOG_FOUND)

if (GLM_FOUND)
  include_directories(${GLM_INCLUDE_DIR})
endif (GLM_FOUND)

if (SDL2_IMAGE_FOUND)
  include_directories(${SDL2_IMAGE_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${SDL2_IMAGE_LIBRARY}")
endif (SDL2_IMAGE_FOUND)

if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${GLEW_LIBRARIES}")
endif()

if (GL_FOUND)
  include_directories(${GL_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES "${GL_gl_LIBRARY}")
endif()

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPPLE")
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg -Wall -Werror")
endif()

if (MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt")
  list(APPEND DEP_LIBRARIES "opengl32")
endif()

add_subdirectory (Source)
#add_subdirectory (RenderingTest)
#add_subdirectory (test-game)
