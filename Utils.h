#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <cstdint>
#include <filesystem>

std::vector<uint8_t> read_bytes(const std::filesystem::path& path);

#endif // _UTILS_H_
