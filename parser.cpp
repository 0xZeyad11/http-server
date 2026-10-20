#include "parser.hpp"
#include <iostream>
#include <sstream>

RequestLine Parser::parseRequestLine(std::string request)
{
    // parsing the method
    HttpMethods method;

    bool valid = true;
    std::string m, p, v;
    if (!request.empty() && request.back() == '\r')
        request.pop_back();

    std::istringstream iss(request);
    std::string extra;
    if (!(iss >> m >> p >> v) || iss >> extra)
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

    if (v.empty() || p.empty() || p[0] != '/' || v.rfind("HTTP/", 0) != 0)
    {
        valid = false;
    }
    else
    {
        if (v.size() < 5)
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
    if (method == "OPTIONS")
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
    case HttpMethods::OPTIONS:
        return "OPTIONS";
    case HttpMethods::HEAD:
        return "HEAD";
    default:
        return "UNKNOWN";
    }
}

/*
========================== HEADERS ===============================
*/

std::string Parser::headerError(HeaderError error)
{
    switch (error)
    {
    case HeaderError::NO_COLON:
        return "malformed";
    case HeaderError::BAD_NAME:
        return "bad-name";
    case HeaderError::FOLD:
        return "fold";
    case HeaderError::LINE_ENDING:
        return "line-ending";
    case HeaderError::HOST:
        return "host";
    case HeaderError::INCOMPLETE:
        return "incomplete";
    default:
        return "malformed";
    }
}

std::string Parser::parseHeaderLine(const std::string &line)
{
    const std::size_t colon_pos = line.find(':');
    if (colon_pos == std::string::npos)
    {
        return "ERR " + headerError(HeaderError::NO_COLON) + ": " + line;
    }

    const std::string name = UTILS::toLower(line.substr(0, colon_pos));
    const std::string value = UTILS::trim(line.substr(colon_pos + 1));

    return name + ": " + value;
}

std::string Parser::parseHeaders()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty())
            break;

        std::cout << parseHeaderLine(line) << '\n';
    }
    return "";
}
