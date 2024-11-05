#pragma once

#include <set>

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
