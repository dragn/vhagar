cmake_minimum_required (VERSION 3.1)
project (GameEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(GAMEENGINE_COMPILE_FLAGS "")

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

set(CMAKE_CXX_STANDARD 11)

add_subdirectory (Vhagar)
add_subdirectory (RenderingTest)
#add_subdirectory (test-game)
