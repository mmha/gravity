find_package(Boost REQUIRED COMPONENTS system)

set(SOURCE_FILES
		HTTPEventStreamBuf.cpp
		Image.cpp
		MappedFile.cpp
)

set(HEADER_FILES
		Subsystem.h
		Attribute.h
		Base64.h
		Engine.h
		Entity.h
		HTTPEventStreamBuf.h
		Image.h
		MappedFile.h
		Sigslot.h
		Stacktrace.h
		)

add_library(GECore STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(GECore PUBLIC bibi::bibi lmi Boost::system gsl::gsl)
set_property(TARGET GECore PROPERTY VERSION ${PROJECT_VERSION})

if(WIN32)
	add_subdirectory(win32)
	target_link_libraries(GECore PRIVATE Dbghelp)
endif()

if(UNIX)
	add_subdirectory(unix)
	target_link_libraries(GECore PRIVATE dl)
endif()

cotire(GECore)
gravity_export_target(GECore ${HEADER_FILES})
