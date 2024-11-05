set(TARGET openapi-yagen-lib-common)

find_package(yaml-cpp REQUIRED)

set(SOURCES
  finalize.h
  node.cpp
  node.h
  node_walker.cpp
  node_walker.h
  std_tools.cpp
  std_tools.h
  string_tools.cpp
  string_tools.h
  yaml_or_json_parser.cpp
  yaml_or_json_parser.h
)

set(DEPS
  yaml-cpp::yaml-cpp
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
