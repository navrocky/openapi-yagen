#include "string_tools.h"

#include <sstream>

using namespace std;

std::vector<std::string_view> operator|(const std::string_view& s, const StringSplitParams& p)
{
    vector<string_view> res;
    string::size_type prevPos = 0, pos = 0;
    while ((pos = s.find(p.delimiter, pos)) != string::npos) {
        res.push_back(s.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }
    res.push_back(s.substr(prevPos, pos - prevPos));
    return res;
}

int64_t operator|(const std::string& s, const ToNumberParams<int64_t>&) { return std::stoll(s); }
int operator|(const std::string& s, const ToNumberParams<int>&) { return std::stoi(s); }
unsigned long operator|(const std::string& s, const ToNumberParams<unsigned long>&) { return std::stoul(s); }

std::string operator|(int i, const ToStringParams&) { return (std::stringstream() << i).str(); }

bool isSpaceOrNewLine(char ch) { return std::isspace(ch) || ch == '\n' || ch == '\r'; }
