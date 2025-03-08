// Container/Include/qstring.hpp
#ifndef QSTRING_HPP
#define QSTRING_HPP
#include <string>

class qstring {
public:
    qstring() = default;
    explicit qstring(const char* str) : data(str) {}
    explicit qstring(const std::string& str) : data(str) {}
    
    const char* c_str() const { return data.c_str(); }
    size_t length() const { return data.length(); }
    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }

    int compare(const qstring& other) const;
    bool operator==(const qstring& other) const;
    qstring to_upper() const;

private:
    std::string data;
};

#endif // QSTRING_HPP