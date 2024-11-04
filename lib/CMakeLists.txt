set(TARGET openapi-yagen-lib)

find_package(inja REQUIRED)
find_package(zip REQUIRED)
find_package(quickjs REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(termcolor REQUIRED)

if(RELEASE_MODE)
    add_link_options(-s -Os)
endif()

set(SOURCES
    common/finalize.h
    common/node.cpp
    common/node.h
    common/node_walker.cpp
    common/node_walker.h
    common/string_tools.cpp
    common/string_tools.h
    common/yaml_or_json_parser.cpp
    common/yaml_or_json_parser.h
    filesystem/definitions.h
    filesystem/dir_file_reader_backend.cpp
    filesystem/dir_file_reader_backend.h
    filesystem/dir_file_writer.cpp
    filesystem/dir_file_writer.h
    filesystem/file_reader.cpp
    filesystem/file_reader.h
    filesystem/file_writer.cpp
    filesystem/file_writer.h
    filesystem/tools.cpp
    filesystem/tools.h
    filesystem/zip_file_reader_backend.cpp
    filesystem/zip_file_reader_backend.h
    js/definitions.h
    js/executor.cpp
    js/executor.h
    js/tools.cpp
    js/tools.h
    logger/console_logger.cpp
    logger/console_logger.h
    logger/logger.cpp
    logger/logger.h
    model/generator_metadata.cpp
    model/generator_metadata.h
    openapi_generator.cpp
    openapi_generator.h
    templates/definitions.h
    templates/inja_template_renderer.cpp
    templates/inja_template_renderer.h
    templates/template_renderer.cpp
    templates/template_renderer.h
)

set(DEPS
    pantor::inja
    zip::zip
    quickjs::quickjs
    yaml-cpp::yaml-cpp
    termcolor::termcolor
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${DEPS})
