add_executable(game main.cpp)
cotire(game)

target_link_libraries(game PRIVATE GECore GESDL GERenderer)

install(
	TARGETS  game
	EXPORT	 "GameTargets"
	LIBRARY  DESTINATION lib
	ARCHIVE  DESTINATION lib
	RUNTIME  DESTINATION bin
	INCLUDES DESTINATION include
)
