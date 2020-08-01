cmake_minimum_required(VERSION 3.16.3)
project(ogss_view_pp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic")


################################
# OGSS/C++ common
################################
ADD_SUBDIRECTORY(lib/ogss.common.cpp)
include_directories(lib/ogss.common.cpp)


################################
# wxWidgets
################################

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})


################################
# GTest
################################

# not now


################################
# Unit Tests
################################

# not now
#file(GLOB_RECURSE TEST_FILES LIST_DIRECTORIES false test/*.cpp)
#add_executable(runTests ${SOURCE_FILES} ${TEST_FILES})

# Link test executable against gtest & gtest_main
#target_link_libraries(runTests ogss.common.cpp gtest gtest_main)
#add_test(runTests runTests)


################################
# The application
################################

#implementation
file(GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false src/*.cpp)
#actual tests


add_executable(ogssview ${SOURCE_FILES})

# Link test executable against gtest & gtest_main
target_link_libraries(ogssview ogss.common.cpp ${wxWidgets_LIBRARIES})

set_property(TARGET ogssview PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
