set(TARGET openapi-yagen-lib-js)

find_package(quickjs REQUIRED)

set(SOURCES
  definitions.h
  executor.cpp
  executor.h
  tools.cpp
  tools.h
)

set(DEPS
  quickjs::quickjs
  openapi-yagen-lib-fs
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
