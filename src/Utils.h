#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <cstdint>
#include <filesystem>
#include <type_traits>

std::vector<uint8_t> read_bytes(const std::filesystem::path& path);

template<class T>
T unpack(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty()) return T{};

    using U = std::make_unsigned_t<T>;
    U u = 0;
    std::size_t n = (bytes.size() < sizeof(T)) ? bytes.size() : sizeof(T);

    // Interpret input bytes as big-endian (bytes[0] is most-significant)
    for (std::size_t i = 0; i < n; ++i)
    {
        u = (u << 8) | static_cast<U>(bytes[i]);
    }

    // If target type is signed and the highest bit of the provided data is set,
    // perform sign-extension when fewer bytes than sizeof(T) were provided.
    if (n < sizeof(T) && std::is_signed<T>::value && (bytes[0] & 0x80u))
    {
        constexpr U allones = ~U(0);
        U mask = allones << (n * 8);
        u |= mask;
    }

    return static_cast<T>(u);
}

template <class T>
std::vector<uint8_t> pack(T input)
{
    using U = std::make_unsigned_t<T>;
    U u = static_cast<U>(input);

    std::vector<uint8_t> out(sizeof(T));
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        out[sizeof(T) - 1 - i] = static_cast<uint8_t>(u & 0xFFu);
        u >>= 8;
    }
    return out;
}

template <class T>
std::vector<T> unpack_vec(const std::vector<uint8_t>& bytes)
{
    std::vector<T> out;
    if (bytes.empty()) return out;

    const std::size_t stride = sizeof(T);
    const std::size_t count = (bytes.size() + stride - 1) / stride;
    out.reserve(count);

    for (std::size_t offset = 0; offset < bytes.size(); offset += stride)
    {
        std::size_t n = ((bytes.size() - offset) < stride) ? (bytes.size() - offset) : stride;
        std::vector<uint8_t> chunk;
        chunk.reserve(n);
        for (std::size_t i = 0; i < n; ++i) chunk.push_back(bytes[offset + i]);
        out.push_back(unpack<T>(chunk));
    }

    return out;
}

template <class T>
std::vector<uint8_t> pack_vec(const std::vector<uint16_t>& bytes)
{
    std::vector<uint8_t> out;
    if (bytes.empty()) return out;

    out.reserve(bytes.size() * sizeof(T));
    for (auto v : bytes) {
        std::vector<uint8_t> p = pack<T>(static_cast<T>(v));
        out.insert(out.end(), p.begin(), p.end());
    }
    return out;
}

#endif // _UTILS_H_
