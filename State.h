//
// Created by Frank Gao on 3/31/21.
//

#ifndef CSP_SOLVER_STATE_H
#define CSP_SOLVER_STATE_H

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;

/**
 * Holds the constraints and the current values/assignments for the variables.
 */
public class State {

    ArrayList<Variable> vars; //Variable objects from input
    ArrayList<Constraint> cons; //Constraint objects from input
    HashMap<Character, Integer> selected; //Keeps track of which values have been chosen for a Variable
    ArrayList<Variable> solvedVars; //Keeps track of the order the Variables are solved in
    boolean useCEP; //true if forward checking is to be used

    /**
     * Constructor for making a default {@code State}.
     */
public State() {
        this(new ArrayList<>(), new ArrayList<>(), false);
    }

    /**
     * Constructor for making a starting {@code State}.
     * @param vars List of Variable Objects
     * @param cons List of Constraint Objects
     * @param useCEP true if forward checking is to be used
     */
public State(ArrayList<Variable> vars, ArrayList<Constraint> cons, boolean useCEP) {
        this.vars = vars;
        this.cons = cons;
        this.useCEP = useCEP;
        selected = new HashMap<>();
        solvedVars = new ArrayList<>();
    }

    /**
     * Creates a copy of a given state.
     * @return copy of given state
     */
public State copyOf() {
        State copy = new State();
        for (Variable v : vars) {
            copy.vars.add(v.copyOf());
        }
        copy.cons = cons;
        copy.selected.putAll(selected);
        for (Variable v : solvedVars) {
            copy.solvedVars.add(v.copyOf());
        }
        copy.useCEP = useCEP;
        return copy;
    }

    /**
     * Checks to see if a given state is a solution
     * @return true if it is a solution
     */
public boolean isSolved() {
        for (Constraint c : cons) {
            //Check if a Variable on a Constraint does not yet have a chosen value
            if (!selected.containsKey(c.var1) || !selected.containsKey(c.var2)) {
                return false;
            }
            int val1 = selected.get(c.var1);
            int val2 = selected.get(c.var2);
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
public void selectNextVar() {
        Collections.sort(vars);
    }

    /**
     * Checks if the current variable selection failed forward check.
     * @return true if failed forward check
     */
public boolean failedFC() {
        return vars.get(0).values.size() == 0;
    }

    /**
     * Sorts the values of a given Variable so the best value is chosen
     * @return array of sorted values for a Variable
     */
public int[] getOrderedVals() {
        Variable nextVar = vars.get(0);
        int[][] valPairs = new int[nextVar.values.size()][2];
        //Matrix which holds the values of a Variable paired with the total affected values if a value is chosen
        for (int i = 0; i < valPairs.length; i++) {
            valPairs[i][0] = nextVar.values.get(i);
            valPairs[i][1] = getAffectedValues(nextVar, valPairs[i][0]);
        }
        //Lambda sort the pairs in increasing order by number of affected values
        Arrays.sort(valPairs, (one, two) -> one[1] - two[1]);
        int[] orderedVals = new int[nextVar.values.size()];
        for (int i = 0; i < valPairs.length; i++) {
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
private int getAffectedValues(Variable nextVar, int nextVal) {
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
public void setVar(int chosenVal) {
        // When forward checking, remove illegal values
        if (useCEP) {
            // Find constraints with the chosen variable
            for (Constraint c : cons) {
                if (c.var1 == vars.get(0).var) {
                    // Find variable it constrains
                    for (Variable v : vars) {
                        if (c.var2 == v.var) {
                            // Find illegal values and remove
                            int i = 0;
                            while (i < v.values.size()) {
                                int val = v.values.get(i);
                                if (!c.valid(chosenVal, val)) {
                                    v.values.remove(i);
                                } else {
                                    i++;
                                }
                            }
                            break;
                        }
                    }
                } else if (c.var2 == vars.get(0).var) {
                    // Find variable it constrains
                    for (Variable v : vars) {
                        if (c.var1 == v.var) {
                            // Find illegal values and remove
                            int i = 0;
                            while (i < v.values.size()) {
                                int val = v.values.get(i);
                                if (!c.valid(val, chosenVal)) {
                                    v.values.remove(i);
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
            if (c.var1 == vars.get(0).var) {
                // Find variable the chosen variable constrains
                for (Variable v : vars) {
                    if (c.var2 == v.var) {
                        // Stop keeping track of constraint
                        v.numConstraints--;
                    }
                }
            } else if (c.var2 == vars.get(0).var) {
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
        selected.put(vars.get(0).var,chosenVal);
        solvedVars.add(vars.get(0));
        vars.remove(0);
    }

    /**
     * Checks to see if a given state is consistent to all of the constraints
     * @return true if it is consistent
     */
public boolean consistent() {
        for (Constraint c : cons) {
            //If either variable in c is not set, move on
            if (!selected.containsKey(c.var1) || !selected.containsKey(c.var2)) {
                continue;
            }

            //Check if constraint is valid
            int val1 = selected.get(c.var1);
            int val2 = selected.get(c.var2);
            if (!c.valid(val1, val2)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Gives the formatted output of the CSP
     * @return String of given output
     */
public String toString() {
        StringBuilder sb = new StringBuilder();
        boolean first = true;
        for (Variable v : solvedVars) {
            if (!first) {
                sb.append(", ");
            }
            first = false;
            sb.append(v.var).append("=").append(selected.get(v.var));
        }
        sb.append("  ");
        if (isSolved()) {
            sb.append("solution");
        } else {
            sb.append("failure");
        }
        return sb.toString();
    }
}

#endif //CSP_SOLVER_STATE_H
