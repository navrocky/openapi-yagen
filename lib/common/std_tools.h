#pragma once

#include <set>
#include <vector>

struct ToSetParams { };
inline ToSetParams toSet() { return {}; }

template <typename Iterable, typename T = Iterable::value_type>
std::set<T> operator|(const Iterable& iterable, const ToSetParams&);

template <typename Mapper>
struct MapToSetParams {
    Mapper mapper;
};
template <typename Mapper>
inline MapToSetParams<Mapper> mapToSet(Mapper&& mapper)
{
    return { std::move(mapper) };
}

template <typename Iterable, typename Mapper, typename T = std::invoke_result_t<Mapper, typename Iterable::value_type>>
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

template <typename Iterable, typename Mapper, typename T = std::invoke_result_t<Mapper, typename Iterable::value_type>>
std::vector<T> operator|(const Iterable& iterable, const MapToVecParams<Mapper>& params)
{
    std::vector<T> res;
    for (const auto& v : iterable) {
        res.push_back(params.mapper(v));
    }
    return res;
}
