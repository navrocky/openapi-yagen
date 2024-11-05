set(TARGET openapi-yagen-lib-templates)

find_package(inja REQUIRED)

set(SOURCES
  definitions.h
  inja_template_renderer.cpp
  inja_template_renderer.h
  template_renderer.cpp
  template_renderer.h
)

set(DEPS
  pantor::inja
  openapi-yagen-lib-fs
  openapi-yagen-lib-logger
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
