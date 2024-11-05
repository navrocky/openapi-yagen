set(TARGET openapi-yagen-lib-logger)

find_package(termcolor REQUIRED)

set(SOURCES
  console_logger.cpp
  console_logger.h
  logger.cpp
  logger.h
)

set(DEPS
  termcolor::termcolor
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
