include(CMakeFindDependencyMacro)

find_dependency(OpenGL)
find_dependency(glbinding)

include("${CMAKE_CURRENT_LIST_DIR}/GERendererTargets.cmake")