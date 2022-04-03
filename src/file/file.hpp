#pragma once

#include <vector>
#include <xstring>

namespace File {
    std::vector<char> readFile(const std::string &filename);
}