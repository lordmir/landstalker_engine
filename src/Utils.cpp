#include "Utils.h"
#include <iterator>
#include <fstream>

std::vector<uint8_t> read_bytes(const std::filesystem::path& path)
{
    std::vector<uint8_t> bytes = {};
    std::ifstream file(path, std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    bytes.reserve(file_size);
    
    bytes.insert(bytes.begin(),
                 std::istream_iterator<uint8_t>(file),
                 std::istream_iterator<uint8_t>());

    return bytes;
}
