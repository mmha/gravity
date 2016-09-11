set(SOURCE_FILES Window.cpp EventHandler.cpp Keyboard.cpp Mouse.cpp SDLException.cpp)
set(HEADER_FILES Window.h EventHandler.h Keyboard.h Mouse.h SDLException.h)

add_library(GESDL STATIC ${SOURCE_FILES} ${HEADER_FILES})
set_property(TARGET GESDL PROPERTY VERSION ${PROJECT_VERSION})
cotire(GESDL)

find_package(SDL2 REQUIRED)
target_link_libraries(GESDL PUBLIC GECore SDL2 GL)

gravity_export_target(GESDL ${HEADER_FILES})