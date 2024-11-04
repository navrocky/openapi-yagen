#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct StringSplitParams {
    std::string_view delimiter;
};
inline StringSplitParams split(const std::string_view& v) { return { v }; }

std::vector<std::string_view> operator|(const std::string_view& s, const StringSplitParams& p);

template <typename T>
struct ToNumberParams { };

template <typename T>
inline ToNumberParams<T> toNumber()
{
    return {};
}

std::int64_t operator|(const std::string& s, const ToNumberParams<std::int64_t>&);
int operator|(const std::string& s, const ToNumberParams<int>&);
unsigned long operator|(const std::string& s, const ToNumberParams<unsigned long>&);

struct ToStringParams { };
inline ToStringParams toString() { return {}; }

std::string operator|(int i, const ToStringParams&);
