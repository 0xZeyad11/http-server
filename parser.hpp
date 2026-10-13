#pragma once

#include <string>
#include <unordered_set>

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

const std::unordered_set<std::string> methods_values = {"GET", "POST", "PUT", "PATCH", "HEAD", "OPTIONS"};

struct RequestLine
{
    HttpMethods method;
    std::string path;
    std::string http_version;
};

struct Headers
{
};

struct Request
{
};

class Parser
{

private:
    bool hasMultipleSpaces(const std::string &line);

public:
    // TODO: change the return type to RequestLine
    RequestLine parseRequestLine(std::string request);
    Parser() = default;
    HttpMethods parseHttpMethod(const std::string &method) const;
    std::string httpMethodToString(const HttpMethods &http) const;
};
