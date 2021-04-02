//
// Created by Frank Gao on 4/1/21.
//

#ifndef CSP_SOLVER_MAIN_H
#define CSP_SOLVER_MAIN_H

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "Variable.h"
#include "Constraint.h"
#include "State.h"

class Driver {
private:
    int iteration;
public:
    void go(std::vector<Variable> vars, std::vector<Constraint> cons, bool useCEP) {
        for (const Constraint &c : cons) {
            for (Variable &v : vars) {
                if (c.var1 == v.var && c.var2 == v.var) {
                    v.numConstraints++;
                }
            }
        }
        iteration++;
        solve(std::make_shared<State>(vars, cons, useCEP));
    }

    bool solve(std::shared_ptr<State> currState) {
        if (currState->isSolved()) {
            if (iteration <= 30) {
                std::cout << std::to_string(iteration) << ". " << currState->to_string() << std::endl;
                return true;
            } else {
                return false;
            }
        }
        currState->selectNextVar();
        if (currState->failedFC()) {
            if (iteration <= 30) {
                std::cout << std::to_string(iteration) << ". " << currState->to_string() << std::endl;
                iteration++;
            }
        }
        for (int val : currState->getOrderedVals()) {
            std::shared_ptr<State> nextState = currState->copyOf();
            nextState->setVar(val);
            if (nextState->consistent()) {
                if (solve(nextState)) {
                    return true;
                }
            } else if (iteration <= 30) {
                std::cout << std::to_string(iteration) << ". " << currState->to_string() << std::endl;
                iteration++;
            } else {
                return false;
            }
        }
        return false;
    }

    static void main(int argc, char *argv[]) {
        if (argc != 4) {
            std::cerr << "Incorrect number of arguments." << std::endl;
            exit(1);
        }
        std::string const varFile = argv[1];
        std::string const conFile = argv[2];
        std::vector<Variable> vars{};
        std::vector<Constraint> cons{};
        bool useCEP;
        {
            std::string line;
            std::ifstream infile(varFile);
            while (std::getline(infile, line)) {
                vars.push_back(Variable(line));
            }
        }
        {
            std::string line;
            std::ifstream infile(varFile);
            while (std::getline(infile, line)) {
                cons.push_back(Constraint(line));
            }
        }
        if (strcmp(argv[3], "none") == 0) {
            useCEP = false;
        } else if (strcmp(argv[3], "fc") == 0) {
            useCEP = true;
        } else {
            std::cerr << "Invalid flag " << argv[3];
            exit(1);
        }
        auto driver = std::make_shared<Driver>();
        driver->go(vars, cons, useCEP);
    }
};


#endif //CSP_SOLVER_MAIN_H
