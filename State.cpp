#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "Constraint.cpp"
#include "Variable.cpp"
#include "string"
#include <sstream>
using namespace std;

/**
 * Holds the constraints and the current values/assignments for the variables.
 */
class State {

public:
    std::vector<Variable> vars;
    std::vector<Constraint> cons;
    std::unordered_map<std::string, int> selected;
    std::vector<Variable> solvedVars;
    bool useCEP;

    /**
     * Constructor for making a default {@code State}.
     */
    State() {
        //  this(new ArrayList<>(), new ArrayList<>(), false);
        vars = {};
        cons = {};
        selected = {};
        solvedVars = {};
        useCEP = false;
    }

    /**
     * Constructor for making a starting {@code State}.
     * @param vars List of Variable Objects
     * @param cons List of Constraint Objects
     * @param useCEP true if forward checking is to be used
     */

    State(vector<Variable> &vars, vector<Constraint> &cons, bool useCEP) {
        vars = vars;
        cons = cons;
        useCEP = useCEP;
        selected = {};
        solvedVars = {};
    };

    /**
     * Creates a copy of a given state.
     * @return copy of given state
     */

    State copyOf() {
        State copy = {};
        for (Variable v : vars) {
            copy.vars.push_back(v.copyOf());
        }
        copy.cons = cons;
        //copy.selected.putAll(selected);
        copy.selected = selected;
        for (Variable v : solvedVars) {
            copy.solvedVars.push_back(v.copyOf());
        }
        copy.useCEP = useCEP;
        return copy;
    }

    /**
     * Checks to see if a given state is a solution
     * @return true if it is a solution
     */
    bool isSolved() {
        for (Constraint c : cons) {
            //Check if a Variable on a Constraint does not yet have a chosen value
            /* if (!selected.find(c.var1) || !selected.find(c.var2)) {
                 return false;
             } */
            int val1 = selected.at(c.var1);
            int val2 = selected.at(c.var2);
            //Check if the Constraint is satisfied, returns false if unsatisfied
            if (!c.valid(val1, val2)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Sorts the Variables so the first one is the Variable to be chosen
     */
    void selectNextVar() {
        std::sort(vars.begin(), vars.end() + 4);
    }

    /**
     * Checks if the current variable selection failed forward check.
     * @return true if failed forward check
     */
    bool failedFC() {
        return vars.at(0).values.size() == 0;
    }


    /**
     * Sorts the values of a given Variable so the best value is chosen
     * @return array of sorted values for a Variable
     */

    int *getOrderedVals() {
        Variable nextVar = vars.at(0);
        int valPairs[nextVar.values.size()][2];

        //Matrix which holds the values of a Variable paired with the total affected values if a value is chosen
        for (int i = 0; i < sizeof(valPairs); i++) {
            valPairs[i][0] = nextVar.values.at(i);
            valPairs[i][1] = getAffectedValues(nextVar, valPairs[i][0]);
        }
        //Lambda sort the pairs in increasing order by number of affected values
        //std::sort(valPairs, (one, two) -> one[1] - two[1]);
        int orderedVals[nextVar.values.size()];
        for (int i = 0; i < sizeof(valPairs); i++) {
            orderedVals[i] = valPairs[i][0];
        }
        return orderedVals;
    }

    /**
     * Finds the amount of values affected by choosing a value of a Variable
     * @param nextVar Variable where the value came from
     * @param nextVal integer of the value chosen
     * @return integer of total affected values
     */
    int getAffectedValues(Variable nextVar, int nextVal) {
        int numAffected = 0;
        //Runs through all Constraints
        for (Constraint c : cons) {
            //Checks if nextVar is the first Variable in the Constraint
            if (c.var1 == nextVar.var) {
                for (Variable v : vars) {
                    //Finds which Variable is the second in the Constraint
                    if (c.var2 == v.var) {
                        //Sees which values in from v are affected by nextVal
                        for (int val : v.values) {
                            if (!c.valid(nextVal, val)) {
                                numAffected++;
                            }
                        }
                        break;
                    }
                }
            }
                //Checks if nextVar is the second Variable in the Constraint
            else if (c.var2 == nextVar.var) {
                for (Variable v : vars) {
                    //Finds which Variable is the first in the Constraint
                    if (c.var1 == v.var) {
                        //Sees which values in from v are affected by nextVal
                        for (int val : v.values) {
                            if (!c.valid(val, nextVal)) {
                                numAffected++;
                            }
                        }
                        break;
                    }
                }
            }
        }
        return numAffected;
    }

    /**
     * Sets the next variable to {@code chosenVal} and updates constraints.
     * @param chosenVal chosen for a move
     */
    void setVar(int chosenVal) {
        // When forward checking, remove illegal values
        if (useCEP) {
            // Find constraints with the chosen variable
            for (Constraint c : cons) {
                if (c.var1 == vars.at(0).var) {
                    // Find variable it constrains
                    for (Variable v : vars) {
                        if (c.var2 == v.var) {
                            // Find illegal values and remove
                            int i = 0;
                            while (i < v.values.size()) {
                                int val = v.values.at(i);
                                if (!c.valid(chosenVal, val)) {
                                    //v.values.remove(i);
                                    //  v.values.erase(i);

                                } else {
                                    i++;
                                }
                            }
                            break;
                        }
                    }
                } else if (c.var2 == vars.at(0).var) {
                    // Find variable it constrains
                    for (Variable v : vars) {
                        if (c.var1 == v.var) {
                            // Find illegal values and remove
                            int i = 0;
                            while (i < v.values.size()) {
                                int val = v.values.at(i);
                                if (!c.valid(val, chosenVal)) {
                                    //  v.values.erase(i);
                                    //v.values.erase(i);
                                } else {
                                    i++;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }

        // Other variables that are constrained to the chosen variable now
        // do not need to keep track of those constraints for least
        // constraining variable
        for (Constraint c : cons) {
            if (c.var1 == vars.at(0).var) {
                // Find variable the chosen variable constrains
                for (Variable v : vars) {
                    if (c.var2 == v.var) {
                        // Stop keeping track of constraint
                        v.numConstraints--;
                    }
                }
            } else if (c.var2 == vars.at(0).var) {
                // Find variable the chosen variable constrains
                for (Variable v : vars) {
                    if (c.var1 == v.var) {
                        // Stop keeping track of constraint
                        v.numConstraints--;
                    }
                }
            }
        }

        // Set variable to chosenVal
        //  selected.set(vars.at(0).var, chosenVal);
        solvedVars.push_back(vars.at(0));
        vars.pop_back();
    }

    /**
     * Checks to see if a given state is consistent to all of the constraints
     * @return true if it is consistent
     */
    bool consistent() {
        for (Constraint c : cons) {
            //If either variable in c is not set, move on
            /*    if (!selected.find(c.var1) || !selected.find(c.var2)) {
                    continue;
                } */

            //Check if constraint is valid
            int val1 = selected.at(c.var1);
            int val2 = selected.at(c.var2);
            if (!c.valid(val1, val2)) {
                return false;
            }
        }
        return true;
    }

/*
     std::string toString() {
        std::stringstream sb;

        bool first = true;
        for (Variable v : solvedVars) {
            if (!first) {
                sb.append(", ");
            }
            first = false;
            sb.append(v.var).append("=").append(selected.find(v.var));
        }
        sb.append("  ");
        if (isSolved()) {
            sb.append("solution");
        } else {
            sb.append("failure");
        }
        return sb.toString();
    }
} */

}