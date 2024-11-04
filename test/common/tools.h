#pragma once

#include <string>

std::string_view getResourcesDir();

std::string getResourcePath(const std::string_view& relativePath);

std::string readResource(const std::string_view& relativePath);
