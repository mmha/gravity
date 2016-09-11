add_library(gsl INTERFACE)
add_library(gsl::gsl ALIAS gsl)
target_include_directories(gsl INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GSL>
	$<INSTALL_INTERFACE:"3rdParty/GSL">
	)

install(TARGETS gsl EXPORT GECoreTargets
        PUBLIC_HEADER DESTINATION include)