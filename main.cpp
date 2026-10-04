#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>

int main() {
    std::unordered_set<std::string> methods = {"GET","POST","PUT","DELETE","HEAD","OPTIONS","PATCH"};
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string m, p, v, extra;
        if (!(iss >> m >> p >> v) || (iss >> extra)) { std::cout << "INVALID\n"; continue; }
        // TODO: validate methods.count(m), p starts with "/", v matches "HTTP/<d>.<d>"
        if (!methods.count(m) || p.empty() || p[0] != '/' || v.rfind("HTTP/", 0) != 0) {
            std::cout << "INVALID\n"; continue;
        }
        std::cout << "METHOD=" << m << " PATH=" << p << " VERSION=" << v << "\n";
    }
}
