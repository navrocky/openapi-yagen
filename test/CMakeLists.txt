set(TARGET openapi-yagen-test)

find_package(Catch2 REQUIRED)

add_definitions(-D__RESOURCES_DIR__=${CMAKE_CURRENT_SOURCE_DIR}/resources -DBUILD_DIR=${CMAKE_BINARY_DIR})

set(SOURCES
    common/main.cpp
    common/tools.cpp
    common/tools.h
    generator_test.cpp
    parser_test.cpp
    vfs_test.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
  Catch2::Catch2
  openapi-yagen-lib
)
