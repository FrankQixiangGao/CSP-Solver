//
// Created by Frank Gao on 3/31/21.
//

#ifndef CSP_SOLVER_VARIABLE_H
#define CSP_SOLVER_VARIABLE_H

#include<vector>
#include "string"

using namespace std;

class Variable {
public:
    basic_string<char, char_traits<char>, allocator<char>> var;
    std::vector<int> values;
    int numConstraints;

    static std::vector<std::string> split(std::string s, std::string const delimiter) {
        std::vector<std::string> r{};
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            r.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        return r;
    }

    Variable() :
            var{'A'},
            values(),
            numConstraints{0} {
    }

    Variable(std::string const input) {
        this->var = input.at(0);
        values = std::vector<int>();
        std::vector<std::string> temp = split(input.substr(3), " ");
        for (const auto s : temp) {
            values.push_back(atoi(s.c_str()));
        }
    }

    int compareTo(Variable other) {
        if (values.size() == other.values.size()) {
            if (numConstraints == other.numConstraints) {
                return var - other.var;
            }
            return other.numConstraints - numConstraints;
        }
        return values.size() - other.values.size();
    }

    Variable copyOf() {
        Variable copy;
        copy.var = this->var;
        for (const auto &item : this->values) {
            copy.values.push_back(item);
        }
        copy.numConstraints = this->numConstraints;
        return copy;
    }

    std::string to_string() const {
        std::string s = std::to_string(static_cast<char>(var)) + ":";
        for (const auto &item : values) {
            s += " " + std::to_string(item);
        }
        return s;
    }
};


#endif //CSP_SOLVER_VARIABLE_H
