#pragma once

#include <memory>

namespace FS {

class FileWriter;
using FileWriterPtr = std::shared_ptr<FileWriter>;

class FileReader;
using FileReaderPtr = std::shared_ptr<FileReader>;

class FileReaderBackend;
using FileReaderBackendPtr = std::shared_ptr<FileReaderBackend>;

class FilePostProcessor;
using FilePostProcessorPtr = std::shared_ptr<FilePostProcessor>;

}
