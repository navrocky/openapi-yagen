set(TARGET openapi-yagen-lib-generator)

find_package(quickjs REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)

set(SOURCES
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

add_library(${TARGET} ${SOURCES}
  schema_validator.h schema_validator.cpp)

target_link_libraries(${TARGET} ${DEPS} nlohmann_json_schema_validator)
