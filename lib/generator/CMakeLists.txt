set(TARGET openapi-yagen-lib-generator)

find_package(quickjs REQUIRED)

set(SOURCES
  functions.cpp
  functions.h
  generator_metadata.cpp
  generator_metadata.h
  openapi_generator.cpp
  openapi_generator.h
)

set(DEPS
  quickjs::quickjs
  openapi-yagen-lib-common
  openapi-yagen-lib-fs
  openapi-yagen-lib-js
  openapi-yagen-lib-templates
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
