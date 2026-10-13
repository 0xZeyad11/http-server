#include "parser.hpp"
#include <iostream>
#include <sstream>

RequestLine Parser::parseRequestLine(std::string request)
{
    // parsing the method
    HttpMethods method;

    bool valid = true;
    std::istringstream stream(request);
    std::string m, p, v, extra;
    stream >> m >> p >> v;
    if (request.empty())
    {
        valid = false;
    }
    if (!request.empty() && request.back() == '\r')
        request.pop_back();

    std::istringstream iss(request);
    if (!(iss >> m >> p >> v) || iss >> extra)
    {
        valid = false;
    }
    if (p.empty() || p[0] != '/' || v.rfind("HTTP/", 0) != 0)
    {
        valid = false;
    }
    method = parseHttpMethod(m);
    if (method == HttpMethods::UNKNOWN || !methods_values.count(m))
    {
        valid = false;
    }

    if (hasMultipleSpaces(request))
    {
        valid = false;
    }
    if (v.empty())
    {
        valid = false;
    }
    else
    {
        std::string version_number = v.substr(5);
        const std::size_t dot = version_number.find('.');
        if (dot == std::string::npos || dot == 0 || dot == version_number.size() - 1)
        {
            valid = false;
        }
        else
        {
            const std::string major = version_number.substr(0, dot);
            const std::string minor = version_number.substr(dot + 1);

            for (const char ch : major)
            {
                if (!std::isdigit(static_cast<unsigned char>(ch)))
                {
                    valid = false;
                }
            }

            for (const char ch : minor)
            {
                if (!std::isdigit(static_cast<unsigned char>(ch)))
                {
                    valid = false;
                }
            }
        }
    }
    if (!valid)
    {
        return RequestLine{HttpMethods::UNKNOWN, "0", "0"}; // convection for invalid request
    }
    return {method, p, v};
}

bool Parser::hasMultipleSpaces(const std::string &line)
{
    for (std::size_t i = 1; i < line.size(); ++i)
    {
        if (line[i] == ' ' && line[i - 1] == ' ')
        {
            return true;
        }
    }

    return false;
}

HttpMethods Parser::parseHttpMethod(const std::string &method) const
{
    if (method == "GET")
        return HttpMethods::GET;
    if (method == "POST")
        return HttpMethods::POST;
    if (method == "DELETE")
        return HttpMethods::DELETE;
    if (method == "PUT")
        return HttpMethods::PUT;
    if (method == "PATCH")
        return HttpMethods::PATCH;
    if (method == "OPTION")
        return HttpMethods::OPTIONS;
    if (method == "HEAD")
        return HttpMethods::HEAD;
    return HttpMethods::UNKNOWN;
}

std::string Parser::httpMethodToString(const HttpMethods &http) const
{
    switch (http)
    {
    case HttpMethods::GET:
        return "GET";
    case HttpMethods::DELETE:
        return "DELETE";
    case HttpMethods::POST:
        return "POST";
    case HttpMethods::PATCH:
        return "PATCH";
    case HttpMethods::PUT:
        return "PUT";
    default:
        return "UNKNOWN";
    }
}