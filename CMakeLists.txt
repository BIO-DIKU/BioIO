cmake_minimum_required (VERSION 2.8)

# About the project
# ------------------------
# Project name
project(BioIO)


# Default options
# ------------------------
# Change default install path
if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()


# Compiler options
# ------------------------
# Thanks to Will Usher for this code snippet:
# http://www.willusher.io/sdl2%20tutorials/2014/03/06/lesson-0-cmake/
#
# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wfloat-equal -Werror -pedantic-errors -std=c++11")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()


# Files in project
# ------------------------
FILE(GLOB SOURCE_FILES src/*.cc src/*.h)
FILE(GLOB INCLUDE_FILES include/BioIO/*.h)

FILE(GLOB TEST_FILES test/*.cc)


# Include and build library
# ------------------------
include_directories("${PROJECT_SOURCE_DIR}/include")
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})


# Build executable (tests) and link library
# ------------------------
add_executable("${PROJECT_NAME}-test" ${TEST_FILES})
target_link_libraries("${PROJECT_NAME}-test" ${PROJECT_NAME})

# Copy fasta test files to build folder
# ------------------------
set(MY_TEST_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/fasta_test_files")
file(MAKE_DIRECTORY ${MY_TEST_DIRECTORY})
file(GLOB FAST_TEST_FILES test/fasta_files/*.fasta)
file(COPY ${FAST_TEST_FILES} DESTINATION ${MY_TEST_DIRECTORY})


# make build types
# ------------------------
ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
  )

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
  )


# Install instructions
# ------------------------
install (DIRECTORY ${PROJECT_SOURCE_DIR}/include/BioIO DESTINATION include)
install (TARGETS ${PROJECT_NAME} DESTINATION lib)

