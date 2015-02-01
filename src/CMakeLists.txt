file(GLOB GameEngine_SRC "**/*.cpp")

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
    LIST(APPEND GAMEENGINE_LIBRARIES ${SDL2_LIBRARY})
endif (SDL2_FOUND)

if (GLOG_FOUND)
    set(GAMEENGINE_COMPILE_FLAGS "${GAMEENGINE_COMPILE_FLAGS} -DWITH_GLOG")
    include_directories(${GLOG_INCLUDE_DIR})
    list(APPEND GAMEENGINE_LIBRARIES"${GLOG_LIBRARY}")
endif (GLOG_FOUND)

if (GLM_FOUND)
    include_directories(${GLM_INCLUDE_DIR})
endif (GLM_FOUND)
    include_directories(${GL_INCLUDE_DIR_GL})
    list(APPEND GAMEENGINE_LIBRARIES "${GL_gl_LIBRARY}")
if (SDL2_IMAGE_FOUND)
    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
    list(APPEND GAMEENGINE_LIBRARIES "${SDL2_IMAGE_LIBRARY}")
endif (SDL2_IMAGE_FOUND)

if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIR})
	list(APPEND GAMEENGINE_LIBRARIES "${GLEW_LIBRARIES}")
endif()

add_executable (GameEngine main.cpp ${GameEngine_SRC})
target_link_libraries (GameEngine ${GAMEENGINE_LIBRARIES})

if (GAME_ENGINE_COMPILE_FLAGS)
	set_target_properties (GameEngine PROPERTIES COMPILE_FLAGS ${GAMEENGINE_COMPILE_FLAGS})
endif()
