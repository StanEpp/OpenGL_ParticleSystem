include(ExternalProject)

set(DEPENDENCY_DIR ${CMAKE_BINARY_DIR}/deps)
set(DEPENDENCY_INSTALL_DIR ${DEPENDENCY_DIR}/installed)
set(DEPENDENCY_INSTALL_LIB_DIR ${DEPENDENCY_INSTALL_DIR}/lib)
set(DEPENDENCY_INSTALL_INCLUDE_DIR ${DEPENDENCY_INSTALL_DIR}/include)

set(DEPENDENCY_INCLUDE_DIR ${CMAKE_BINARY_DIR}/deps/installed/include PARENT_SCOPE)

externalproject_add(glm_ext
    PREFIX ${DEPENDENCY_DIR}/glm
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                    <SOURCE_DIR>/glm ${DEPENDENCY_INSTALL_INCLUDE_DIR}/glm
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    UPDATE_DISCONNECTED 1
    URL https://github.com/g-truc/glm/archive/0.9.9-a2.zip
)

externalproject_add(glfw_ext
    PREFIX ${DEPENDENCY_DIR}/glfw
    CMAKE_ARGS -DGLFW_BUILD_TESTS=OFF
               -DGLFW_BUILD_EXAMPLES=OFF
               -DGLFW_BUILD_DOCS=OFF
               -DGLFW_INSTALL=ON
               -DBUILD_SHARED_LIBS=ON
               -DCMAKE_INSTALL_PREFIX=${DEPENDENCY_INSTALL_DIR}
    UPDATE_COMMAND ""
    UPDATE_DISCONNECTED 1
    URL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
)

add_library(glfw SHARED IMPORTED GLOBAL)

set_target_properties(glfw PROPERTIES IMPORTED_LOCATION
  "${DEPENDENCY_INSTALL_LIB_DIR}/${CMAKE_SHARED_MODULE_PREFIX}glfw${CMAKE_SHARED_LIBRARY_SUFFIX}"
)