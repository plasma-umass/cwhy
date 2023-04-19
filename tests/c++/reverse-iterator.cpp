#include <algorithm>
#include <string>

std::string trim(std::string s) {
    static const auto isNotSpace = [](auto c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), isNotSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace), s.end());
    return s;
}
