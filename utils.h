#ifndef UTILS_H
#define UTILS_H
#include <cstdint>
#include <utility>

inline size_t CalculateId(const size_t boardSize, const size_t row, const size_t column)
{
    return row * boardSize + column;
}

inline std::pair<size_t, size_t> DecodeId(const size_t boardSize, const size_t id)
{
    return { id / boardSize, id % boardSize };
}

#endif // UTILS_H
