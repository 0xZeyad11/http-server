#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <sstream>

bool hasMultipleSpaces(std::string_view line) {
    for (std::size_t i = 1; i < line.size(); ++i) {
        if (line[i] == ' ' && line[i - 1] == ' ') {
            return true;
        }
    }

    return false;
}

int main() {
    std::unordered_set<std::string> methods = {"GET","POST","PUT","DELETE","HEAD","OPTIONS","PATCH"};
    std::string line;
    while (std::getline(std::cin, line)) {
        bool valid = true ;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string m, p, v, extra;
        if (!(iss >> m >> p >> v) || (iss >> extra)) { std::cout << "INVALID\n"; continue; }
        if(hasMultipleSpaces(line)){
            std::cout << "INVALID\n"; continue;
        }
        if (!methods.count(m) || p.empty() || p[0] != '/' || v.rfind("HTTP/", 0) != 0 ) {
            std::cout << "INVALID\n"; continue;
        }else {

            std::string_view version_number = v.substr(5);
            const std::size_t dot = version_number.find('.');
            if (dot == std::string::npos || dot == 0 || dot == version_number.size() - 1){
                valid = false ;
            }else {
                const std::string_view major= version_number.substr(0 , dot);
                const std::string_view minor = version_number.substr(dot + 1);


                for (const char ch: major){
                    if(!std::isdigit(static_cast<unsigned char> (ch))){
                        valid = false ;
                    }
                }

                for (const char ch: minor){
                    if(!std::isdigit(static_cast<unsigned char> (ch))){
                        valid = false ;
                    }
                }

            }
            if(!valid){
                std::cout << "INVALID\n";
                continue ;
            }
        }


        std::cout << "METHOD=" << m << " PATH=" << p << " VERSION=" << v << "\n";
    }
}
