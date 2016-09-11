add_subdirectory(Executable)
add_subdirectory(Core)
add_subdirectory(Renderer)
add_subdirectory(SDL)

install(DIRECTORY . DESTINATION include/gravity COMPONENT devel FILES_MATCHING PATTERN "*.h")
install(
		FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/GravityConfig.cmake"
		DESTINATION ${ConfigPackageLocation}
		COMPONENT devel
)
install(DIRECTORY cmake DESTINATION "${ConfigPackageLocation}/modules" COMPONENT devel FILES_MATCHING PATTERN "*.cmake")