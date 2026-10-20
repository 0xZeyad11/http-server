#pragma once

#include <string>
#include <unordered_set>
#include <algorithm>
#include <cctype>

enum class HttpMethods
{
    GET,
    POST,
    PUT,
    PATCH,
    OPTIONS,
    DELETE,
    HEAD,
    UNKNOWN
};

namespace UTILS
{
    inline std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return str;
    }

    inline std::string trim(std::string str)
    {
        const std::size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos)
            return "";
        const std::size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    }
}

enum class HeaderError
{
    FOLD,
    LINE_ENDING,
    NO_COLON,
    BAD_NAME,
    INCOMPLETE,
    HOST
};

const std::unordered_set<std::string> methods_values = {"GET", "POST", "PUT", "PATCH", "HEAD", "OPTIONS"};

struct RequestLine
{
    HttpMethods method;
    std::string path;
    std::string http_version;
};

class Parser
{
public:
    /*
    ======================== REQUEST LINE =============================
    */
    RequestLine parseRequestLine(std::string request);
    Parser() = default;
    HttpMethods parseHttpMethod(const std::string &method) const;
    std::string httpMethodToString(const HttpMethods &http) const;

    /*
    ========================== HEADERS ===============================
    */
    std::string parseHeaders();

private:
    bool hasMultipleSpaces(const std::string &line);

    std::string headerError(HeaderError error);
    std::string parseHeaderLine(const std::string &line);
};
