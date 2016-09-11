file(GLOB_RECURSE INSTALLED_TARGETS "${CMAKE_CURRENT_LIST_DIR}/*Targets.cmake")
file(GLOB USED_MACROS "${CMAKE_CURRENT_LIST_DIR}/modules/cmake/*.cmake")

foreach(target ${INSTALLED_TARGETS})
	include(${target})
endforeach()

foreach(macro ${USED_MACROS})
	include(${macro})
endforeach()