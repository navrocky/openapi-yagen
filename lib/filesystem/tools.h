#pragma once

#include "definitions.h"

namespace FS {

std::string readFile(const std::string& filePath);
FileReaderBackendPtr openAutodetectedFileSystem(const std::string& path);

}
