file(GLOB GameEngine_SRC "**/*.cpp")

set(GAMEENGINE_COMPILE_FLAGS "-g -std=c++11")

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(Glog)
find_package(GLM REQUIRED)
find_package(GL REQUIRED)

# Setup dependencies
if (SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    LIST(APPEND GAMEENGINE_LIBRARIES ${SDL2_LIBRARIES})
endif (SDL2_FOUND)

if (GLOG_FOUND)
    set(GAMEENGINE_COMPILE_FLAGS "${GAMEENGINE_COMPILE_FLAGS} -DWITH_GLOG")
    include_directories(${GLOG_INCLUDE_DIR})
    list(APPEND "${GLOG_LIBRARY}")
endif (GLOG_FOUND)

if (GLM_FOUND)
    include_directories(${GLM_INCLUDE_DIR})
endif (GLM_FOUND)

if (GL_FOUND)
    message("GL FOUND! ${GL_INCLUDE_DIR_GL}")
    include_directories(${GL_INCLUDE_DIR_GL})
    list(APPEND "${GL_gl_LIBRARY}")
endif (GL_FOUND)

if (SDL2_Image_FOUND)
    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
    list(APPEND "${SDL2_IMAGE_LIBRARY}")
endif (SDL2_Image_FOUND)

add_executable (GameEngine main.cpp ${GameEngine_SRC})
target_link_libraries (GameEngine ${GAMEENGINE_LIBRARIES})
set_target_properties (GameEngine PROPERTIES COMPILE_FLAGS ${GAMEENGINE_COMPILE_FLAGS})
