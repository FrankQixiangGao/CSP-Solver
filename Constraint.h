//
// Created by Frank Gao on 3/31/21.
//

#ifndef CSP_SOLVER_CONSTRAINT_H
#define CSP_SOLVER_CONSTRAINT_H


/**
 * Object representation of a constraint.
 */
public class Constraint {
    char var1; //First Variable of the constraint
    char var2; //Second Variable of the constraint
    char operator; //Equality operator of the Variables

    /**
     * Constructor for making a default {@code Constraint}.
     */
public Constraint() {
        var1 = 'A';
        var2 = 'A';
        operator = '=';
    }

    /**
     * Constructor for making a new {@code Constraint}.
     */
public Constraint(String input) {
        var1 = input.charAt(0);
        var2 = input.charAt(4);
        operator = input.charAt(2);
    }

    /**
     * Checks to see if a constraint works on a given pair of values
     * @param val1 value of the first Variable
     * @param val2 value of the second Variable
     * @return true if the pair of values works under the constraint
     */
public boolean valid(int val1, int val2) {
        switch (operator) {
                case '<':
                return val1 < val2;
                case '>':
                return val1 > val2;
                case '!':
                return val1 != val2;
                case '=':
                return val1 == val2;
        }
            return false;
    }

    @Override
public String toString() {
        return String.format("%c %c %c", var1, operator, var2);
    }
}


#endif //CSP_SOLVER_CONSTRAINT_H
