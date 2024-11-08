set(TARGET openapi-yagen-lib-fs)

find_package(zip REQUIRED)

set(SOURCES
  definitions.h
  dir_file_reader_backend.cpp
  dir_file_reader_backend.h
  dir_file_writer.cpp
  dir_file_writer.h
  file_post_processor.cpp
  file_post_processor.h
  file_reader.cpp
  file_reader.h
  file_writer.cpp
  file_writer.h
  remote_file_reader_backend.cpp
  remote_file_reader_backend.h
  tools.cpp
  tools.h
  zip_file_reader_backend.cpp
  zip_file_reader_backend.h
)

set(DEPS
  zip::zip
  openapi-yagen-lib-common
  openapi-yagen-lib-logger
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
