macro (gravity_export_target TARGET_NAME HEADER_FILES)
	install(
	  FILES 		${HEADER_FILES}
	  DESTINATION	include
	  COMPONENT		devel
	)

	install(
		TARGETS  ${TARGET_NAME}
		EXPORT	 "${TARGET_NAME}Targets"
	 	LIBRARY  DESTINATION lib
	  	ARCHIVE  DESTINATION lib
	  	RUNTIME  DESTINATION bin
	  	INCLUDES DESTINATION include
	)

	write_basic_package_version_file(
			"${CMAKE_CURRENT_BINARY_DIR}/Gravity/${TARGET_NAME}ConfigVersion.cmake"
			VERSION ${GE_VERSION}
			COMPATIBILITY AnyNewerVersion
	)

	export(EXPORT "${TARGET_NAME}Targets"
		   FILE "${CMAKE_CURRENT_BINARY_DIR}/Gravity/${TARGET_NAME}Targets.cmake"
		   NAMESPACE GE::
		   )

	configure_file("${TARGET_NAME}Config.cmake"
				   "${CMAKE_CURRENT_BINARY_DIR}/Gravity/${TARGET_NAME}Config.cmake"
				   @ONLY
				   )

	install(EXPORT "${TARGET_NAME}Targets"
			FILE "${TARGET_NAME}Targets.cmake"
			DESTINATION ${ConfigPackageLocation}
			NAMESPACE GE::
			)
	install(
			FILES
				"${CMAKE_CURRENT_BINARY_DIR}/Gravity/${TARGET_NAME}Config.cmake"
				"${CMAKE_CURRENT_BINARY_DIR}/Gravity/${TARGET_NAME}ConfigVersion.cmake"
			DESTINATION ${ConfigPackageLocation}
			COMPONENT devel
	)
endmacro()