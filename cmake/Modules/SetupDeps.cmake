set(3RDPARTY "${CMAKE_SOURCE_DIR}/../vhagar-deps" CACHE STRING "3rd party libraries folder")

add_definitions(-DVH_DEPS_FOLDER=${3RDPARTY})

set(ENV{GLM_ROOT_DIR} "${3RDPARTY}/glm")
set(ENV{SDL2DIR} "${3RDPARTY}/SDL2-2.0.5")
set(ENV{SDL2TTFDIR} "${3RDPARTY}/SDL2_ttf-2.0.14")
set(ENV{SDL2IMAGEDIR} "${3RDPARTY}/SDL2_image-2.0.1")
set(glog_DIR "${3RDPARTY}/glog/Debug/lib/cmake/glog")
set(gflags_DIR "${3RDPARTY}/gflags/CMake")
set(lpp_DIR "${3RDPARTY}/LivePP")

list(APPEND CMAKE_PREFIX_PATH
  "${3RDPARTY}/glew"
  )
  
# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glog 0.3.4 REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glew REQUIRED)
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
  list(APPEND DEP_LIBRARIES GLEW::GLEW)
endif()

if (OpenGL_FOUND)
  include_directories(${GL_INCLUDE_DIR})
  list(APPEND DEP_LIBRARIES OpenGL::GL)
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
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt /FUNCTIONPADMIN /OPT:NOREF /OPT:NOICF /DEBUG:FASTLINK")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  list(APPEND DEP_LIBRARIES "opengl32")
  list(APPEND DEP_LIBRARIES ${SDL2MAIN_LIBRARY} $<$<CONFIG:Debug>:msvcrtd> $<$<CONFIG:Release>:msvcrt>)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

include_directories(${lpp_DIR}/API)