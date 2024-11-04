#pragma once

#include <stdexcept>
#include <string>
#include <type_traits>

#include "node.h"
#include "string_tools.h"

class WalkError : public std::invalid_argument {
public:
    WalkError(const std::string& message, const std::string& path);
};

template <typename T>
T nodeAs(const Node&);

class NodeWalker {
public:
    NodeWalker(const Node& v, const std::string& basePath = std::string());

    NodeWalker walk(const std::string& path) const;
    inline NodeWalker operator[](const std::string& path) const { return walk(path); }

    bool isEmpty() const;

    template <typename T = Node>
    std::optional<T> optional() const;

    template <typename T>
    T required() const;

    template <typename Mapper, typename T = typename std::invoke_result<Mapper, const NodeWalker&>::type>
    std::vector<T> requiredList(const Mapper& mapper) const;

    template <typename Mapper, typename T = typename std::invoke_result<Mapper, const NodeWalker&>::type>
    std::optional<std::vector<T>> optionalList(const Mapper& mapper) const;

    template <typename Mapper, typename T = typename std::invoke_result<Mapper, const NodeWalker&>::type>
    T requiredObj(const Mapper& mapper) const;

    template <typename Mapper, typename T = typename std::invoke_result<Mapper, const NodeWalker&>::type>
    std::optional<T> optionalObj(const Mapper& mapper) const;

private:
    void requireValue() const;
    std::string joinPath(const std::string_view& left, const std::string_view& right) const;

    Node v;
    std::string basePath;
};

template <typename Mapper, typename T>
inline std::optional<T> NodeWalker::optionalObj(const Mapper& mapper) const
{
    if (isEmpty())
        return std::nullopt;
    return mapper(*this);
}

template <typename Mapper, typename T>
T NodeWalker::requiredObj(const Mapper& mapper) const
{
    requireValue();
    return mapper(*this);
}

template <typename Mapper, typename T>
inline std::optional<std::vector<T>> NodeWalker::optionalList(const Mapper& mapper) const
{
    if (isEmpty())
        return std::nullopt;
    auto list = required<Node::Vec>();
    std::vector<T> res;
    res.reserve(list.size());
    for (unsigned int i = 0; i < list.size(); i++) {
        res.push_back(mapper(NodeWalker(list[i], joinPath(basePath, i | toString()))));
    }
    return res;
}

template <typename Mapper, typename T>
std::vector<T> NodeWalker::requiredList(const Mapper& mapper) const
{
    requireValue();
    return optionalList(mapper);
}

template <typename T>
inline std::optional<T> NodeWalker::optional() const
{
    if (isEmpty())
        return std::nullopt;
    try {
        return nodeAs<T>(v);
    } catch (const std::exception& e) {
        throw WalkError(e.what(), basePath);
    }
}

template <typename T>
inline T NodeWalker::required() const
{
    requireValue();
    return optional<T>().value();
}

template <>
inline Node nodeAs<Node>(const Node& v)
{
    return v;
}
