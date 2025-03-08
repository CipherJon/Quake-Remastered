// Container/Include/qstack.hpp
#ifndef QSTACK_HPP
#define QSTACK_HPP
#include <vector>
#include "Quake/Exception/error.h"

template<typename T, typename Container = std::vector<T>>
class qstack {
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;

    qstack() = default;
    explicit qstack(const Container& cont) : data(cont) {}
    explicit qstack(Container&& cont) : data(std::move(cont)) {}

    void push(const T& value) { data.push_back(value); }
    void push(T&& value) { data.push_back(std::move(value)); }

    template<typename... Args>
    void emplace(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
    }

    void pop() {
        if (empty()) throw quake::runtime_error("qstack is empty");
        data.pop_back();
    }

    reference top() {
        if (empty()) throw quake::runtime_error("qstack is empty");
        return data.back();
    }

    const_reference top() const {
        if (empty()) throw quake::runtime_error("qstack is empty");
        return data.back();
    }

    bool empty() const noexcept { return data.empty(); }
    size_type size() const noexcept { return data.size(); }
    void swap(qstack& other) noexcept { data.swap(other.data); }

private:
    Container data;
};

#endif // QSTACK_HPP