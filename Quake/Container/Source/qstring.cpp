// Container/Source/qstring.cpp
#include "Container/Include/qstring.hpp"
#include <algorithm>
#include <iterator>

int qstring::compare(const qstring& other) const {
    return data.compare(other.data);
}

bool qstring::operator==(const qstring& other) const {
    return data == other.data;
}

qstring qstring::to_upper() const {
    std::string upper_str;
    upper_str.reserve(data.size());
    std::transform(data.begin(), data.end(), std::back_inserter(upper_str),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return qstring(upper_str);
}