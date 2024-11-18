#pragma once

#include <algorithm>
#include <cstdint>
#include <sstream>
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

template <typename T>
std::string operator|(const T& t, const ToStringParams&) {
    return (std::stringstream() << t).str();
}

bool isSpaceOrNewLine(char ch);

template <typename Func>
concept CharTestFunc = requires(Func f) {
    { f((char)'a') } -> std::same_as<bool>;
};

template <CharTestFunc Func>
struct LeftTrimParams {
    Func testChar;
};
template <CharTestFunc Func>
inline LeftTrimParams<Func> ltrim(Func&& testChar)
{
    return { std::move(testChar) };
}
inline auto ltrim(char ch) -> auto
{
    return LeftTrimParams { [ch](char curCh) { return ch == curCh; } };
}

template <CharTestFunc Func>
std::string operator|(const std::string& t, const LeftTrimParams<Func>& params)
{
    auto res = t;
    auto it = std::find_if(res.begin(), res.end(), [&](auto ch) { return !params.testChar(ch); });
    res.erase(res.begin(), it);
    return res;
}

template <CharTestFunc Func>
struct RightTrimParams {
    Func testChar;
};
template <CharTestFunc Func>
inline RightTrimParams<Func> rtrim(Func&& testChar)
{
    return { std::move(testChar) };
}
template <CharTestFunc Func>
std::string operator|(const std::string& s, const RightTrimParams<Func>& params)
{
    auto res = s;
    std::string::const_iterator it
        = std::find_if(res.rbegin(), res.rend(), [&](auto ch) { return !params.testChar(ch); });
    res.erase(it, res.end());
    return res;
}

template <CharTestFunc Func>
struct TrimParams {
    Func testChar;
};
template <CharTestFunc Func>
inline TrimParams<Func> trim(Func&& testChar)
{
    return { std::move(testChar) };
}

inline auto trim() -> auto { return TrimParams { isSpaceOrNewLine }; }

template <CharTestFunc Func>
std::string operator|(const std::string& s, const TrimParams<Func>& params)
{
    auto res = s;
    auto leftIt = std::find_if(res.begin(), res.end(), [&](auto ch) { return !params.testChar(ch); });
    auto rightIt = std::find_if(res.rbegin(), res.rend(), [&](char ch) { return !params.testChar(ch); }).base();
    res.erase(rightIt, res.end());
    res.erase(res.begin(), leftIt);
    return res;
}

struct JoinToStringParams {
    const std::string_view& delimiter;
};
inline JoinToStringParams joinToString(const std::string_view& delimiter) { return { delimiter }; }

template <typename Iterable>
std::string operator|(const Iterable& iterable, const JoinToStringParams& params)
{
    std::stringstream ss;
    bool first = true;
    for (const auto& v : iterable) {
        if (first)
            first = false;
        else
            ss << params.delimiter;
        ss << v;
    }
    return ss.str();
}
