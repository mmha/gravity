enable_testing()

add_executable(${PROJECT_NAME}_test testmain.cpp)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${GTEST_INCLUDE_DIRS} "../src")
target_link_libraries(${PROJECT_NAME}_test PRIVATE gtest_main gtest ${PROJECT_NAME}_lib)