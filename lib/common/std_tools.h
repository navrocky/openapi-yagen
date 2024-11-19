#pragma once

#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename Iterable>
using IterableValue = std::remove_cvref_t<decltype(*std::declval<Iterable>().begin())>;

struct ToSetParams { };
inline ToSetParams toSet() { return {}; }

template <typename Iterable, typename T = IterableValue<Iterable>>
std::set<T> operator|(const Iterable& iterable, const ToSetParams&)
{
    return std::set<T>(iterable.begin(), iterable.end());
}

struct ToVectorParams { };
inline ToVectorParams toVector() { return {}; }

template <typename Iterable, typename T = IterableValue<Iterable>>
std::vector<T> operator|(const Iterable& iterable, const ToVectorParams&)
{
    return std::vector<T>(iterable.begin(), iterable.end());
}

template <typename Mapper>
struct MapToSetParams {
    Mapper mapper;
};
template <typename Mapper>
inline MapToSetParams<Mapper> mapToSet(Mapper&& mapper)
{
    return { std::move(mapper) };
}

template <typename Iterable, typename Mapper, typename T = std::invoke_result_t<Mapper, IterableValue<Iterable>>>
std::set<T> operator|(const Iterable& iterable, const MapToSetParams<Mapper>& params)
{
    std::set<T> res;
    for (const auto& v : iterable) {
        res.insert(params.mapper(v));
    }
    return res;
}

template <typename Mapper>
struct MapToVecParams {
    Mapper mapper;
};
template <typename Mapper>
inline MapToVecParams<Mapper> mapToVector(Mapper&& mapper)
{
    return { std::move(mapper) };
}

template <typename Iterable, typename Mapper, typename T = std::invoke_result_t<Mapper, IterableValue<Iterable>>>
std::vector<T> operator|(const Iterable& iterable, const MapToVecParams<Mapper>& params)
{
    std::vector<T> res;
    for (const auto& v : iterable) {
        res.push_back(params.mapper(v));
    }
    return res;
}

struct FirstOrThrowParams { };
inline FirstOrThrowParams firstOrThrow() { return {}; }

template <typename Iterable, typename T = IterableValue<Iterable>>
T operator|(const Iterable& iterable, const FirstOrThrowParams&)
{
    if (iterable.empty())
        throw std::runtime_error("<216313e7> Iterable is empty");
    return *iterable.begin();
}
