#include "string_tools.h"

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

bool isSpaceOrNewLine(char ch) { return std::isspace(ch) || ch == '\n' || ch == '\r'; }

std::string operator|(const std::string& s, const AnsiToLowerParams& params)
{
    auto lowered = s;
    std::transform(s.begin(), s.end(), lowered.begin(), [](char c) { return (char)std::tolower(c); });
    return lowered;
}
